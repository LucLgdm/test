/**********************************************************************
 * In a first terminal, lunch the server:
 * make lvl5 && ./main
 * In a second terminal, you can connect to the server using:
 * curl http://localhost:8080/
 * This is a server HTTP minimal.
 * Objective : read a request HTTP and send an answer formatted
 **********************************************************************/

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <vector>
#include <poll.h>
#include <csignal>
#include <cerrno>
#include <pthread.h>
#include <sstream>

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



int main(){

	signal(SIGINT, handle_sigint);
	pthread_t exit_thread;
	pthread_create(&exit_thread, NULL, wait_for_exit, NULL);


	int server_fd, client_fd;
	struct sockaddr_in address;
	
	init_server(&server_fd, &address);
	
	socklen_t addrlen = sizeof(address);

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
				client_fd = accept(server_fd, (struct sockaddr *)&address, &addrlen);
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
				if (fds[i].revents & POLLIN) {
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
						const char body[] = "Hello, world!\nThis is a minimal HTTP server.\nComment est votre blanquette ?\nElle est bonne\n";
						std::ostringstream oss;
						oss << "HTTP/1.1 200 OK\r\n"
							<< "Content-Type: text/plain\r\n"
							<< "Content-Length: " << strlen(body) << "\r\n"
							<< "\r\n"
							<< body;

						send(client_fd, oss.str().c_str(), oss.str().size(), 0);
						close(fds[i].fd);
						fds.erase(fds.begin() + i);
						i--;
						cout << "Response sent" << endl;
					}
				}
			}
			
			usleep(100000);
		}
	}
	pthread_join(exit_thread, NULL);

	for (size_t i = 0; i < fds.size(); ++i)
        close(fds[i].fd);
    std::cout << "Serveur arrêté proprement." << std::endl;
	return 0;
}