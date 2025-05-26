#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>


#define PORT 8080
#define BUFF_SIZE 256


int main(int argc, char* argv[]) {

	int client_fd = 0;
	struct sockaddr_in serv_addr;
	char buff[BUFF_SIZE] = {0};

	// Create socket
	client_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (client_fd < 0) {

		perror("Socket creation error");
		return -1;

	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	// Convert IPv4 address from text to binary
	if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {

		perror("Invalid address / Address not supported");
		return -1;

	}

	// Connect
	if (connect(client_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
		
		perror("Connection failed");
		return -1;

	}

	char server_ip[INET_ADDRSTRLEN];
    	inet_ntop(AF_INET, &serv_addr.sin_addr, server_ip, INET_ADDRSTRLEN);
    	printf("Server connected: %s:%d\n", server_ip, ntohs(serv_addr.sin_port));

	// Communicate
	while (1) {

		// Receive message from server
		memset(buff, 0, BUFF_SIZE);
		read(client_fd, buff, BUFF_SIZE);
		printf("Server Respond: %s\n", buff);
	
		if (strcmp(buff, "Good Bye") == 0) {

			break;

		}

		// Send message to server
		char* msg = (char*)calloc(BUFF_SIZE, sizeof(char*));
		printf("Enter message to send server: ");
		fgets(msg, BUFF_SIZE, stdin);
		msg[strcspn(msg, "\n")] = '\0';
		send(client_fd, msg, strlen(msg), 0);
		free(msg);

	}

	close (client_fd);

	return 0;

}

