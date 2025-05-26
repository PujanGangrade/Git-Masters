#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>


#define PORT 8080
#define BUFF_SIZE 256


int main(int argc, char* argv[]) {

	int server_fd, new_socket;
	char buff[BUFF_SIZE];
	
	struct sockaddr_in address;
	int addrlen = sizeof(address);

	//Socket Create
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd == 0) {

		perror("Socket failed");
		exit(EXIT_FAILURE);

	}

	//Set address and port
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	// Bind socket
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {

		perror("Bind failed");
		exit(EXIT_FAILURE);
	
	}

	// Listen
	if (listen(server_fd, 3) < 0) {

		perror("Listen failed");
		exit(EXIT_FAILURE);

	}

	// Accept Connection
	new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
	if (new_socket < 0) {

		perror("Accept failed");
		exit(EXIT_FAILURE);

	}

	char client_ip[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &address.sin_addr, client_ip, INET_ADDRSTRLEN);
	printf("Client connected: %s:%d\n", client_ip, ntohs(address.sin_port));


	// Communicate
	while (1) {
		
		// Send Message to Client
		char* msg = (char*)calloc(BUFF_SIZE, sizeof(char));
		printf("Enter the message to send client: ");
		fgets(msg, BUFF_SIZE, stdin);
		msg[strcspn(msg, "\n")] = '\0';
		send(new_socket, msg,  strlen(msg), 0);
		free(msg);


		// Receive Message from Client
		memset(buff, 0, BUFF_SIZE);
		read(new_socket, buff, BUFF_SIZE);
		printf("Client Respond: %s\n", buff);
		
		if (strcmp(buff, "/quit") == 0) {

			send(new_socket, "Good Bye", strlen("Good Bye"), 0);
			break;

		}

	}

	//Socket close
	close(new_socket);
	close(server_fd);

	return 0;

}
