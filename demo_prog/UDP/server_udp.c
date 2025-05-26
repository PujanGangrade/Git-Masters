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

	int socket_fd;
	char buff[BUFF_SIZE];

	struct sockaddr_in servaddr, cliaddr;

	// Create socket
	if ((socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {

		perror("Socket creation failed");
		exit(EXIT_FAILURE);

	}

	// Setup server address
	memset(&servaddr, 0, sizeof(servaddr));
	memset(&cliaddr, 0, sizeof(cliaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(PORT);


	// Bind socket
	if (bind(socket_fd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {

		perror("Bind failed");
		exit(EXIT_FAILURE);

	}

	socklen_t len;
	int n;

	while (1) {

		len= sizeof(cliaddr);
		memset(buff, 0, BUFF_SIZE);

		// Receive message
		n = recvfrom(socket_fd, buff, BUFF_SIZE, 0, (struct sockaddr *)&cliaddr, &len);
		buff[n] = '\0';
		printf("Client: %s\n", buff);

		// Exit condition
		if (strcmp(buff, "/quit") == 0) {
			
			char *msg = "Good Bye";
			sendto(socket_fd, msg, strlen(msg), 0, (struct sockaddr *)&cliaddr, len);
			break;
	
		}

		// Respond
		char msg[BUFF_SIZE];
		printf("Enter response to client: ");
		fgets(msg, BUFF_SIZE, stdin);
		msg[strcspn(msg, "\n")] = '\0';

		sendto(socket_fd, msg, strlen(msg), 0, (const struct sockaddr *)&cliaddr, len);

	}

	close(socket_fd);
	return 0;
			
}
