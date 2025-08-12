/**********************************************************************
 * In a first terminal, lunch the server:
 * make lvl5 && ./main
 * In a second terminal, you can connect to the server using:
 * nc localhost 8080
 * This is the chat Room minimal : a client send a message to the server
 * and this one send it to all the other client
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


int main(){
	// Quit gracefully
	signal(SIGINT, handle_sigint);
	pthread_t exit_thread;
	pthread_create(&exit_thread, NULL, wait_for_exit, NULL);


	int server_fd, client_fd;
	struct sockaddr_in address;
	socklen_t addrlen = sizeof(address);
	const int port = 8080;

	// INITIALIZATION
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd < 0) {
		cerr << "Failed to create socket" << endl;
		exit(EXIT_FAILURE);
	}

	memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);

	int opt = 1;
	// Set socket options to reuse the address
	// This is useful to avoid "Address already in use" error when restarting the server
	// after a short time
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
		std::cerr << "setsockopt failed" << std::endl;
		close(server_fd);
		exit(EXIT_FAILURE);
	}


	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
		cerr << "Failed to bind socket" << endl;
		close(server_fd);
		exit(EXIT_FAILURE);
	}

	if (listen(server_fd, 5) < 0) {
		cerr << "Failed to listen on socket" << endl;
		close(server_fd);
		exit(EXIT_FAILURE);
	}

	vector<struct pollfd> fds;
	struct pollfd pfd;
	pfd.fd = server_fd;
	pfd.events = POLLIN;
	pfd.revents = 0;
	fds.push_back(pfd);
	// fds.push_back({server_fd, POLLIN, 0}); Non authorized in C++98...

	while(!stop_flag){
		int ret = poll(fds.data(), fds.size(), 100); // -1 means infinite timeout, timeout 100ms
		if (ret < 0) {
			if (errno == EINTR) {
				continue; // Interrupted by a signal, retry
			} else {
				cerr << "Poll failed" << endl;
				close(server_fd);
				exit(EXIT_FAILURE);
			}
		} else if (ret == 0) { // Timeout so never called here
			continue; // No events
		} else { // At least one fd is ready to be read or written
			if (fds[0].revents & POLLIN) { // Socket server is ready to accept a new connection
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
				if (fds[i].revents & POLLIN && fds[i].fd != server_fd) { // Check if it's a client socket
					char buffer[1024];
					int bytes_read = recv(fds[i].fd, buffer, sizeof(buffer), 0);
					if (bytes_read <= 0) {
						if (bytes_read == 0)
							cout << "Client disconnected" << endl;
						else	
							cerr << "Failed to read from socket" << endl;
						close(fds[i].fd);
						fds.erase(fds.begin() + i);
						i--; // Adjust index after removal
						continue ;
					}
					for (int j = 1; j < (int)fds.size(); j++){
						if (j != i){
							if (send(fds[j].fd, buffer, bytes_read, 0) < 0) {
								cerr << "Failed to send clients" << endl;
								close(fds[j].fd);
								fds.erase(fds.begin() + j);
								if (j < i) i--;
								j--;
							}
						}
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