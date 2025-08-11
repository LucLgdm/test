/*************************************************************
 * In a first terminal, lunch the server:
 * make lvl2 && ./main
 * In a second terminal, you can connect to the server using:
 * nc localhost 8080
 * The server can handle multiple clients with multiprocessing
 *************************************************************/

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
using namespace std;

void	handle_client(int client_fd, uint16_t port) {
	char buffer[1024];
	int bytes_read = 1;
	while(bytes_read > 0){
		bytes_read = recv(client_fd, buffer, sizeof(buffer), 0);
		if (bytes_read < 0) {
			cerr << "Failed to read from socket" << endl;
			break;
		} else if (bytes_read == 0) {
			cout << "Client disconnected" << endl;
			break;
		}
		cout << "[Client " << port << "]: " << string(buffer, bytes_read);
	}
	close(client_fd);
}

void sigchld_handler(int) {
    // Nettoyage des processus fils zombies
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

int main(){
	int server_fd, client_fd;
	struct sockaddr_in address;
	socklen_t addrlen = sizeof(address);
	const int port = 8080;

	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd < 0){
		cerr << "Failed to create socket" << endl;
		exit(EXIT_FAILURE);
	}

	memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);

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

	while(1){
		client_fd = accept(server_fd, (struct sockaddr *)&address, &addrlen);
		if (client_fd < 0) {
			cerr << "Failed to accept connection" << endl;
			continue;
		}
		char client_ip[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &(address.sin_addr), client_ip, INET_ADDRSTRLEN);
		cout << "Client connected from " << client_ip << ":" << ntohs(address.sin_port) << endl;

		pid_t pid = fork();
		if (pid < 0) {
			cerr << "Failed to fork process" << endl;
			close(client_fd);
		}else if (pid == 0){
			close (server_fd); // Child process does not need the server socket
			handle_client(client_fd, ntohs(address.sin_port));
			exit(0);
		}else {
			close(client_fd);
		}

	}
	close (server_fd);
	return 0;
}