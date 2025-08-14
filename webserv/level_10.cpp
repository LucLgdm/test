/*
 * level_10.cpp — Mini-CGI HTTP Server (C++98, poll, fork, execve, pipe)
 *
 * Features:
 *   - Serves static files for regular GET requests (like previous levels)
 *   - If the requested path is /script, executes a script (script.sh or script.py)
 *     and returns its output as the HTTP response body (Mini-CGI)
 *   - Uses fork(), pipe(), and execve() to run the script and capture its output
 *   - Non-blocking I/O with poll(), supports multiple clients
 *   - Properly closes connections and handles errors (404, 500, etc.)
 *
 * Usage:
 *   1. Place a script named 'script.sh' or 'script.py' in the same directory.
 *   2. Compile: g++ -Wall -Wextra -pedantic -std=c++98 level_10.cpp -o webserv
 *   3. Run: ./webserv
 *   4. Test: curl http://localhost:8080/script
 *
 * This file is self-contained and does not require external libraries beyond POSIX.
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>
#include <errno.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

#define PORT 8080
#define MAX_CLIENTS 100
#define BUFFER_SIZE 4096
#define TIMEOUT 10000 // ms

volatile bool stop_flag = false;

void handle_sigint(int signum) {
	(void)signum;
	stop_flag = true;
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
	int client_fd, pipe_fd[2];
	socklen_t addrlen = sizeof(*address);
	struct pollfd pfd;
	pfd.fd = server_fd;
	pfd.events = POLLIN;
	pfd.revents = 0;

	while (!stop_flag) {
		client_fd = accept(server_fd, (struct sockaddr *)address, &addrlen);
		if (client_fd < 0) {
			if (errno == EAGAIN || errno == EWOULDBLOCK) {
				usleep(100000); // Attendre 100ms et réessayer
				continue;
			} else {
				cerr << "Failed to accept connection" << endl;
				close(server_fd);
				exit(EXIT_FAILURE);
			}
		}
		log_event(client_fd, "ACCEPT", 0);

		char buffer[BUFFER_SIZE];
		int bytes_read = 1;
			bytes_read = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
			if (bytes_read < 0) {
				if (errno == EAGAIN || errno == EWOULDBLOCK) { log_event(client_fd, "RECV_FAIL", 0);}
				else {
					cerr << "Failed to read from socket" << endl;
					close(client_fd);
					log_event(client_fd, "CLOSE", 0);
					break;
				}
			} else if (bytes_read == 0){
					log_event(client_fd, "DISCONNECT", 0);
					close(client_fd);
					log_event(client_fd, "CLOSE", 0);
					break;
			} else {
				buffer[bytes_read] = '\0';
				log_event(client_fd, "RECV", bytes_read);
				char method[8], url[1024];
				sscanf(buffer, "%s %s", method, url);
				if (strcmp(method, "GET") != 0) {
					cerr << "Unsupported method: " << method << endl;
					const char *response = "HTTP/1.1 501 Not Implemented\r\n\r\n";
					send(client_fd, response, strlen(response), 0);
					close(client_fd);
					log_event(client_fd, "CLOSE", 0);
					continue;
				}

				if (strcmp(url, "/script.py") == 0) {
					pipe(pipe_fd);
					pid_t pid = fork();
					if (pid < 0) {
						cerr << "Fork failed" << endl;
						const char *response = "HTTP/1.1 500 Internal Server Error\r\n\r\n";
						send(client_fd, response, strlen(response), 0);
						close(client_fd);
						log_event(client_fd, "CLOSE", 0);
						continue;
					} else if (pid == 0) { // Child process
						close(pipe_fd[0]); // Close read end
						dup2(pipe_fd[1], STDOUT_FILENO); // Redirect stdout to pipe
						close(pipe_fd[1]); // Close write end after dup2
						char *args[] = {(char *)"./script.py", NULL}; // Change to your script name
						execve(args[0], args, NULL);
						cerr << "Execve failed" << endl;
						exit(EXIT_FAILURE);
					} else { // Parent process
						close(pipe_fd[1]); // Close write end
						read(pipe_fd[0], buffer, sizeof(buffer) - 1);
						close(pipe_fd[0]); // Close read end after reading
						waitpid(pid, NULL, 0); // Wait for child to finish
					}
					buffer[bytes_read] = '\0';
					log_event(client_fd, "SCRIPT_OUTPUT", bytes_read);
					string type = "text/plain"; // Assuming script output is plain text
					ostringstream oss;
					oss << "HTTP/1.1 200 OK\r\n"
						<< "Content-Type: " << type << "\r\n"
						<< "Content-Length: " << bytes_read << "\r\n"
						<< "\r\n"
						<< buffer;
					send(client_fd, oss.str().c_str(), oss.str().size(), 0);
					log_event(client_fd, "SEND", oss.str().size());
					close(client_fd);
					log_event(client_fd, "CLOSE", 0);
				} else {
					strcpy(url, default_file);
				}
			}
	}
	close(client_fd);
	close(server_fd);
	log_event(client_fd, "CLOSE", 0);
	log_event(server_fd, "CLOSE", 0);
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