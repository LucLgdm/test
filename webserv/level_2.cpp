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

using namespace std;

int main(){
	int server_fd, client_fd;
	struct sockaddr_in address;
	socklen_t addrlen = sizeof(address);
	const int port = 8080;

	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd < 0) {
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
	client_fd = accept(server_fd, (struct sockaddr *)&address, &addrlen);
	if (client_fd < 0) {
		cerr << "Failed to accept connection" << endl;
		close(server_fd);
		exit(EXIT_FAILURE);
	}
	cout << "Client connected" << endl;

	char buffer[1024];
	int bytes_read = 1;
	while(bytes_read > 0) {
		bytes_read = recv(client_fd, buffer, sizeof(buffer), 0);
		if (bytes_read < 0) {
			cerr << "Failed to read from socket" << endl;
			break;
		} else if (bytes_read == 0) {
			cout << "Client disconnected" << endl;
			break;
		}
		// send(client_fd, buffer, bytes_read, 0);

		// recv() doesn't put the \0 terminator, so we need to add it
		cout << "Received: " << string(buffer, bytes_read);
		memset(buffer, 0, sizeof(buffer)); // Clear the buffer for the next read
	}

	close(client_fd);
	close(server_fd);
	return 0;
}