/**************************************************************
 * In a first terminal, lunch the server:
 * make lvl1 && ./main
 * In a second terminal, you can connect to the server using:
 * nc localhost 8080
 * The serveur send a message to the client
 **************************************************************/

#include <iostream>
#include <cstring> 			// For memset
#include <cstdlib> 			// For exit
#include <unistd.h> 		// For close
#include <sys/socket.h>
#include <netinet/in.h> 	// For sockaddr_in, AF_INET, etc.



int main(){
	int server_fd, client_fd;
	struct sockaddr_in address;
	socklen_t addrlen = sizeof(address);
	const int port = 8080; // Define the port number
	
	/*******************
	 * Open a socket
	 * *****************/

	// AF_INET is used for IPv4 addresses (internet protocol)
	// SOCK_STREAM is used for TCP connections
	// 0 is the protocol, which is usually set to 0 for TCP/IP
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd < 0) {
		std::cerr << "Failed to create socket" << std::endl;
		exit(EXIT_FAILURE);
	}
	std::cout << "Socket created successfully" << std::endl;

	/*******************
	 * Bind the socket
	 * *****************/

	// Initialize the address structure to store the address information
	std::memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET; // IPv4
	address.sin_addr.s_addr = INADDR_ANY; // Bind to any address
	address.sin_port = htons(port); // Port number 8080

	// Bind the socket to the address and port
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
		std::cerr << "Failed to bind socket" << std::endl;
		close(server_fd);
		exit(EXIT_FAILURE);
	}
	std::cout << "Socket bound to port " << port << std::endl;

	/*************************
	 * Listen for connections
	 * ***********************/

	// Listen for incoming connections, with a backlog of 3
	if (listen(server_fd, 3) < 0) {
		std::cerr << "Failed to listen on socket" << std::endl;
		close(server_fd);
		exit(EXIT_FAILURE);
	}
	std::cout << "Listening for connections on port " << port << std::endl;

	/*************************
	 * Accept connections
	 * ***********************/

	client_fd = accept(server_fd, (struct sockaddr *)&address, &addrlen);
	if (client_fd < 0) {
		std::cerr << "Failed to accept connection" << std::endl;
		close(server_fd);
		exit(EXIT_FAILURE);
	}
	std::cout << "Connection accepted" << std::endl;

	/*************************
	 * Send message to client
	 * ***********************/

	const char *message = "Hello from server!\nThis is a simple TCP server example using C++98.\n";
	if (send(client_fd, message, std::strlen(message), 0) < 0) {
		std::cerr << "Failed to send message" << std::endl;
		close(client_fd);
		close(server_fd);
		exit(EXIT_FAILURE);
	}
	std::cout << "Message sent to client: " << message << std::endl;

	/*************************
	 * Close the sockets
	 * ***********************/
	close(client_fd);
	close(server_fd);
	std::cout << "Sockets closed" << std::endl;
	std::cout << "Server terminated successfully" << std::endl;
	return 0;
}