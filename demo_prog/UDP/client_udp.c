#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFF_SIZE 256

int main() {

	int socket_fd;
	char buff[BUFF_SIZE];
	struct sockaddr_in servaddr;

	// Create server address
	memset(&servaddr, 0, sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);
	//servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_addr.s_addr = INADDR_ANY;

	socklen_t len = sizeof(servaddr);

	if ((socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {

		perror("Socket creation failed");
		exit(EXIT_FAILURE);

	}

	while (1) {

		char msg[BUFF_SIZE];
		printf("Enter message to send to server: ");
		fgets(msg, BUFF_SIZE, stdin);
		msg[strcspn(msg, "\n")]= '\0';


		// Send to server
		sendto(socket_fd, msg, strlen(msg), 0, (const struct sockaddr *)&servaddr, len);


		// Exit condition
		if (strcmp(msg, "/quit") == 0) {
		
			recvfrom(socket_fd, buff, BUFF_SIZE, 0, NULL, NULL);
			buff[BUFF_SIZE - 1] = '\0';
			printf("Server: %s\n", buff);
			break;
			
		}
		
		// Receive response
		memset(buff, 0, BUFF_SIZE);
		recvfrom(socket_fd, buff, BUFF_SIZE, 0, NULL, NULL);
		printf("Server: %s\n", buff);

	}

	close(socket_fd);
	return 0;

}

