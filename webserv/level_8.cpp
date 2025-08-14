/**********************************************************************
 * Non-blocking HTTP server (C++98, poll, single-threaded)
 *
 * - Sert des fichiers statiques du disque (GET /fichier)
 * - Gère plusieurs clients simultanés avec poll()
 * - Envoie les fichiers en plusieurs morceaux (non bloquant)
 * - Ferme explicitement chaque connexion après la réponse (Connection: close)
 * - Log chaque événement important (ACCEPT, RECV, 404, CLOSE...)
 * - Gère la déconnexion propre des clients (navigateur, curl, nc...)
 *
 * Utilisation :
 *   1. make lvl8 && ./main /chemin/absolu/fichier.html
 *   2. curl http://localhost:8080/ ou ouvrir dans un navigateur
 **********************************************************************/

 #include <iostream>
#include <cstring>
 #include <netinet/in.h>
#include <vector>
#include <poll.h>
#include <csignal>
#include <cerrno>
#include <pthread.h>
#include <sstream>
#include <fcntl.h>
#include <sys/stat.h>
#include <map>
#include <ctime>

using namespace std;

volatile bool stop_flag = false;

// Structure pour gérer l'état d'un client
struct Client {
	int fd;
	int file_fd;
	off_t file_offset;
	off_t file_size;
	bool sending;
	string header;
	Client(int fd_) : fd(fd_), file_fd(-1), file_offset(0), file_size(0), sending(false) {}
};

void handle_sigint(int signum) {
	(void)signum;
	stop_flag = 1;
}

void* wait_for_exit(void *arg) {
	(void)arg;
	cout << "Tape 'q' puis Entrée pour quitter proprement.\n";
	char c;
	while (cin >> c) {
		if (c == 'q') {
			stop_flag = true;
			break;
		}
	}
	return NULL;
}


void log_event(int fd, const char* type, ssize_t size) {
	time_t now = time(NULL);
	char buf[32];
	strftime(buf, sizeof(buf), "%H:%M:%S", localtime(&now));
	cout << "[" << buf << "] " << type << " fd=" << fd << " size=" << size << endl;
}

// Fonction pour gérer l'envoi du fichier (retourne true si terminé, false sinon)
bool send_file_chunk(Client &cli, struct pollfd &fdinfo) {
	char file_buffer[1024];
	ssize_t bytes_read_file = pread(cli.file_fd, file_buffer, sizeof(file_buffer), cli.file_offset);
	if (bytes_read_file <= 0) {
		close(cli.file_fd);
		close(cli.fd);
		return true;
	}
	ssize_t total_sent = 0;
	while (total_sent < bytes_read_file) {
		ssize_t bytes_sent = send(cli.fd, file_buffer + total_sent, bytes_read_file - total_sent, 0);
		if (bytes_sent < 0) {
			if (errno == EAGAIN || errno == EWOULDBLOCK) {
				// Socket non prêt, on attendra le prochain POLLOUT
				fdinfo.events = POLLOUT;
				return false;
			} else {
				close(cli.file_fd);
				close(cli.fd);
				return true;
			}
		}
		log_event(cli.fd, "SEND", bytes_sent);
		total_sent += bytes_sent;
		cli.file_offset += bytes_sent;
	}
	if (cli.file_offset >= cli.file_size) {
		close(cli.file_fd);
		close(cli.fd);
		return true;
	}
	fdinfo.events = POLLOUT;
	return false;
}



void init_server(int *server_fd, struct sockaddr_in *address) {
	const int port = 8080;

	*server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (*server_fd < 0) {
		cerr << "Failed to create socket" << endl;
		exit(EXIT_FAILURE);
	}
	// Set the socket to non-blocking mode
	// This allows the server to handle multiple clients without blocking
	int flags = fcntl(*server_fd, F_GETFL, 0);
	if (flags == -1) { perror("fcntl F_GETFL"); exit(1); }
	if (fcntl(*server_fd, F_SETFL, flags | O_NONBLOCK) == -1) {
		perror("fcntl F_SETFL");
		exit(1);
	}

	memset(address, 0, sizeof(*address));
	address->sin_family = AF_INET;
	address->sin_addr.s_addr = INADDR_ANY;
	address->sin_port = htons(port);

	int opt = 1;
	if (setsockopt(*server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
		cerr << "setsockopt failed" << endl;
		close(*server_fd);
		exit(EXIT_FAILURE);
	}

	if (bind(*server_fd, (struct sockaddr *)address, sizeof(*address)) < 0) {
		cerr << "Failed to bind socket" << endl;
		close(*server_fd);
		exit(EXIT_FAILURE);
	}

	if (listen(*server_fd, 5) < 0) {
		cerr << "Failed to listen on socket" << endl;
		close(*server_fd);
		exit(EXIT_FAILURE);
	}
}

string get_content_type(const string &path) {
	if (path.size() >= 5 && path.substr(path.size()-5) == ".html") return "text/html";
	if (path.size() >= 4 && path.substr(path.size()-4) == ".txt")  return "text/plain";
	if (path.size() >= 4 && path.substr(path.size()-4) == ".css")  return "text/css";
	if (path.size() >= 3 && path.substr(path.size()-3) == ".js")   return "application/javascript";
	if (path.size() >= 4 && path.substr(path.size()-4) == ".jpg")  return "image/jpeg";
	if (path.size() >= 4 && path.substr(path.size()-4) == ".png")  return "image/png";
	return "application/octet-stream";
}



void run_server_loop(int server_fd, struct sockaddr_in *address, char *default_file) {

	int client_fd;
	socklen_t addrlen = sizeof(*address);
	vector<struct pollfd> fds;
	vector<Client> clients;
	struct pollfd pfd;
	pfd.fd = server_fd;
	pfd.events = POLLIN;
	pfd.revents = 0;
	fds.push_back(pfd);
	clients.push_back(Client(server_fd)); // dummy pour aligner les index

	while(!stop_flag){
		int ret = poll(fds.data(), fds.size(), 100);
		if (ret < 0) {
			if (errno == EINTR) {
				continue;
			} else {
				cerr << "Poll failed" << endl;
				close(server_fd);
				exit(EXIT_FAILURE);
			}
		} else if (ret == 0) {
			continue;
		} else {
			// Accept new clients
			if (fds[0].revents & POLLIN) {
				client_fd = accept(server_fd, (struct sockaddr *)address, &addrlen);
				if (client_fd < 0) {
					cerr << "Failed to accept connection" << endl;
					close(server_fd);
					exit(EXIT_FAILURE);
				}
				int cflags = fcntl(client_fd, F_GETFL, 0);
				if (cflags == -1) { perror("fcntl F_GETFL client"); close(client_fd); continue; }
				if (fcntl(client_fd, F_SETFL, cflags | O_NONBLOCK) == -1) {
					perror("fcntl F_SETFL client");
					close(client_fd);
					continue;
				}
				log_event(client_fd, "ACCEPT", 0);
				struct pollfd pfc;
				pfc.fd = client_fd;
				pfc.events = POLLIN;
				pfc.revents = 0;
				fds.push_back(pfc);
				clients.push_back(Client(client_fd));
			}

			// Parcours des clients
			for(int i = 1; i < (int)fds.size(); i++) {
				Client &cli = clients[i];
				// Si on est en train d'envoyer un fichier (POLLOUT)
				if (cli.sending && (fds[i].revents & POLLOUT)) {
					bool done = send_file_chunk(cli, fds[i]);
					if (done) {
						fds.erase(fds.begin() + i);
						clients.erase(clients.begin() + i);
						i--;
					}
					continue;
				}

				// Si on reçoit une requête (POLLIN)
				if (fds[i].revents & POLLIN && !cli.sending) {
					char buffer[1024];
					int bytes_read = recv(cli.fd, buffer, sizeof(buffer), 0);
					if (bytes_read < 0) {
						if (errno == EAGAIN || errno == EWOULDBLOCK) {
							log_event(cli.fd, "RECV_FAIL", 0);
							close(cli.fd);
							fds.erase(fds.begin() + i);
							clients.erase(clients.begin() + i);
							i--;
						}
					} else if (bytes_read == 0) {
						log_event(cli.fd, "DISCONNECT", 0);
						close(cli.fd);
						fds.erase(fds.begin() + i);
						clients.erase(clients.begin() + i);
						i--;
					} else {
						buffer[bytes_read] = '\0';
						log_event(cli.fd, "RECV", bytes_read);

						char method[8], url[1024];
						sscanf(buffer, "%s %s", method, url);
						if (strcmp(url, "/") == 0) { strcpy(url, default_file); }
						string type = get_content_type(url);
						char path[1024];
						snprintf(path, sizeof(path), ".%s", url);
						int file_fd = open(path, O_RDONLY);
						if (file_fd < 0) {
							ostringstream oss;
							oss << "HTTP/1.1 404 Not Found\r\n"
								<< "Content-Type: " << type << "\r\n"
								<< "Content-Length: 13\r\n"
								<< "\r\n"
								<< "404 Not Found";
							send(cli.fd, oss.str().c_str(), oss.str().size(), 0);
							log_event(cli.fd, "404", 0);
							close(cli.fd);
							fds.erase(fds.begin() + i);
							clients.erase(clients.begin() + i);
							i--;
						} else {
							struct stat file_stat;
							fstat(file_fd, &file_stat);
							ostringstream oss;
							oss << "HTTP/1.1 200 OK\r\n"
								<< "Content-Type: " << type << "\r\n"
								<< "Content-Length: " << file_stat.st_size << "\r\n"
								<< "\r\n";
							send(cli.fd, oss.str().c_str(), oss.str().size(), 0);
							log_event(cli.fd, "SEND_HEADER", oss.str().size());
							cli.file_fd = file_fd;
							cli.file_offset = 0;
							cli.file_size = file_stat.st_size;
							cli.sending = true;
							fds[i].events = POLLOUT;
						}
					}
				}
			}
		}
	}
	for (size_t i = 0; i < fds.size(); ++i)
		close(fds[i].fd);
	cout << "Serveur arrêté proprement." << endl;
}

int main(int argc, char *argv[]) {
	if (argc != 2) {
		cerr << "Usage: ./main </file.html>" << endl;
		return EXIT_FAILURE;
	}

	char *default_file = argv[1];
	if (default_file[0] != '/') {
		cerr << "Error: Default file must be an absolute path." << endl;
		return EXIT_FAILURE;
	}

	signal(SIGINT, handle_sigint);
	pthread_t exit_thread;
	pthread_create(&exit_thread, NULL, wait_for_exit, NULL);

	int server_fd;
	struct sockaddr_in address;
	init_server(&server_fd, &address);

	run_server_loop(server_fd, &address, default_file);

	pthread_join(exit_thread, NULL);
	return 0;
}
