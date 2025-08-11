/*****************************************************************
 * In a first terminal, lunch the server:
 * make lvl2 && ./main
 * In a second terminal, you can connect to the server using:
 * nc localhost 8080
 * The client can send messages to the server
 * and the server echoes them back or prints them to the console
 *****************************************************************/

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>


int main(){
	int server_fd, client_fd;
	struct sockaddr_in address;
	socklen_t addrlen = sizeof(address);
	const int port = 8080;

	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd < 0) {
		std::cerr << "Failed to create socket" << std::endl;
		exit(EXIT_FAILURE);
	}

	std::memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);

	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
		std::cerr << "Failed to bind socket" << std::endl;
		close(server_fd);
		exit(EXIT_FAILURE);
	}

	if (listen(server_fd, 5) < 0) {
		std::cerr << "Failed to listen on socket" << std::endl;
		close(server_fd);
		exit(EXIT_FAILURE);
	}
	client_fd = accept(server_fd, (struct sockaddr *)&address, &addrlen);
	if (client_fd < 0) {
		std::cerr << "Failed to accept connection" << std::endl;
		close(server_fd);
		exit(EXIT_FAILURE);
	}
	std::cout << "Client connected" << std::endl;

	char buffer[1024];
	int bytes_read = 1;
	while(bytes_read > 0) {
		bytes_read = recv(client_fd, buffer, sizeof(buffer), 0);
		if (bytes_read < 0) {
			std::cerr << "Failed to read from socket" << std::endl;
			break;
		} else if (bytes_read == 0) {
			std::cout << "Client disconnected" << std::endl;
			break;
		}
		// send(client_fd, buffer, bytes_read, 0);

		// recv() doesn't put the \0 terminator, so we need to add it
		std::cout << "Received: " << std::string(buffer, bytes_read);
		std::memset(buffer, 0, sizeof(buffer)); // Clear the buffer for the next read
	}

	close(client_fd);
	close(server_fd);
	return 0;
}