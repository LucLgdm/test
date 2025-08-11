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
using namespace std;


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
		cerr << "Failed to create socket" << endl;
		exit(EXIT_FAILURE);
	}
	cout << "Socket created successfully" << endl;

	/*******************
	 * Bind the socket
	 * *****************/

	// Initialize the address structure to store the address information
	memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET; // IPv4
	address.sin_addr.s_addr = INADDR_ANY; // Bind to any address
	address.sin_port = htons(port); // Port number 8080

	// Bind the socket to the address and port
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
		cerr << "Failed to bind socket" << endl;
		close(server_fd);
		exit(EXIT_FAILURE);
	}
	cout << "Socket bound to port " << port << endl;

	/*************************
	 * Listen for connections
	 * ***********************/

	// Listen for incoming connections, with a backlog of 3
	if (listen(server_fd, 3) < 0) {
		cerr << "Failed to listen on socket" << endl;
		close(server_fd);
		exit(EXIT_FAILURE);
	}
	cout << "Listening for connections on port " << port << endl;

	/*************************
	 * Accept connections
	 * ***********************/

	client_fd = accept(server_fd, (struct sockaddr *)&address, &addrlen);
	if (client_fd < 0) {
		cerr << "Failed to accept connection" << endl;
		close(server_fd);
		exit(EXIT_FAILURE);
	}
	cout << "Connection accepted" << endl;

	/*************************
	 * Send message to client
	 * ***********************/

	const char *message = "Hello from server!\nThis is a simple TCP server example using C++98.\n";
	if (send(client_fd, message, strlen(message), 0) < 0) {
		cerr << "Failed to send message" << endl;
		close(client_fd);
		close(server_fd);
		exit(EXIT_FAILURE);
	}
	cout << "Message sent to client: " << message << endl;

	/*************************
	 * Close the sockets
	 * ***********************/
	close(client_fd);
	close(server_fd);
	cout << "Sockets closed" << endl;
	cout << "Server terminated successfully" << endl;
	return 0;
}