/**********************************************************************
 * In a first terminal, lunch the server:
 * make lvl5 && ./main
 * In a second terminal, you can connect to the server using:
 * curl http://localhost:8080/
 * This is a static HTTP server.
 * Objective : read a file from the disk and send it to the client
 **********************************************************************/

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <vector>
#include <poll.h>
#include <csignal>
#include <cerrno>
#include <pthread.h>
#include <sstream>
#include <fcntl.h>
#include <sys/stat.h>

using namespace std;

volatile bool stop_flag = false;

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

void init_server(int *server_fd, struct sockaddr_in *address) {
	const int port = 8080;

	*server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (*server_fd < 0) {
		cerr << "Failed to create socket" << endl;
		exit(EXIT_FAILURE);
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

std::string get_content_type(const std::string &path) {
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
	struct pollfd pfd;
	pfd.fd = server_fd;
	pfd.events = POLLIN;
	pfd.revents = 0;
	fds.push_back(pfd);

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
			if (fds[0].revents & POLLIN) {
				client_fd = accept(server_fd, (struct sockaddr *)address, &addrlen);
				if (client_fd < 0) {
					cerr << "Failed to accept connection" << endl;
					close(server_fd);
					exit(EXIT_FAILURE);
				}
				cout << "Client connected" << endl;
				struct pollfd pfc;
				pfc.fd = client_fd;
				pfc.events = POLLIN;
				pfc.revents = 0;
				fds.push_back(pfc);
			}

			for(int i = 1; i < (int)fds.size(); i++) {
				if (fds[i].revents & POLLIN && fds[i].fd != server_fd) { // Check if it's a client socket
					char buffer[1024];
					int bytes_read = recv(fds[i].fd, buffer, sizeof(buffer), 0);
					if (bytes_read < 0) {
						cerr << "Failed to read from socket" << endl;
						close(fds[i].fd);
						fds.erase(fds.begin() + i);
						i--;
					} else if (bytes_read == 0) {
						cout << "Client disconnected" << endl;
						close(fds[i].fd);
						fds.erase(fds.begin() + i);
						i--;
					} else {
						buffer[bytes_read] = '\0';
						cout << "Received: " << buffer << endl;

						char method[8], url[1024];
						sscanf(buffer, "%s %s", method, url); // Method = GET, url = /path/to/file

						if (strcmp(url, "/") == 0) { strcpy(url, default_file); } // Default file
						string type = get_content_type(url);

						char path[1024];
						snprintf(path, sizeof(path), ".%s", url); // Construction of the file path

						int file_fd = open(path, O_RDONLY);
						if (file_fd < 0) {
							ostringstream oss;
							oss << "HTTP/1.1 404 Not Found\r\n"
								<< "Content-Type: " << type << "\r\n"
								<< "Content-Length: 13\r\n"
								<< "\r\n"
								<< "404 Not Found";
							send(fds[i].fd, oss.str().c_str(), oss.str().size(), 0);
						}else {
							// Get the file size
							struct stat file_stat;
							fstat(file_fd, &file_stat);

							// Send HTTP response header
							std::ostringstream oss;
							oss << "HTTP/1.1 200 OK\r\n"
								<< "Content-Type: " << type << "\r\n"
								<< "Content-Length: " << file_stat.st_size << "\r\n"
								<< "\r\n";
							send(fds[i].fd, oss.str().c_str(), oss.str().size(), 0);

							// Send HTTP response body
							char file_buffer[1024];
							int bytes_read_file = 1;
							while (bytes_read_file > 0) {
								bytes_read_file = read(file_fd, file_buffer, sizeof(file_buffer));
								send(fds[i].fd, file_buffer, bytes_read_file, 0);
							}
							close(file_fd);
						}
					}
				}
			}


			usleep(100000);
		}
	}
	for (size_t i = 0; i < fds.size(); ++i)
		close(fds[i].fd);
	std::cout << "Serveur arrêté proprement." << std::endl;
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
