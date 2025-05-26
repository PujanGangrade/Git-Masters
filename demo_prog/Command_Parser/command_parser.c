#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define USER_NAME 16
#define FILE_NAME 128
#define DATA_SIZE 256

// Enum for command types using bit flag
typedef enum {
	BROADCAST = 1,
	MSG = 2, 
	FILE_SEND = 4,
	LIST = 8,
	QUIT = 16,
} CommandType;

// Pack the struct with 1-byte alignment to avoid padding
#pragma pack(push, 1)
typedef struct {

	unsigned char command_type : 5;
	char user_name[USER_NAME];
	char file_name[FILE_NAME];
	char data[DATA_SIZE];

} client_msg;
#pragma pack(pop)


void input_parser(const char *input, client_msg *msg) {

	char buffer[DATA_SIZE];
	strncpy(buffer, input, DATA_SIZE - 1);
	buffer[DATA_SIZE - 1] = '\0';


	// Initialize struct
	msg->command_type = 0;
	msg->user_name[0] = '\0';
	msg->file_name[0] = '\0';
	msg->data[0] = '\0';

	// Detext command or plain message
	if (buffer[0] != '/') {

		// Broadcasr message
		msg->command_type = BROADCAST;
		strncpy(msg->data, buffer, DATA_SIZE - 1);
		msg->data[DATA_SIZE - 1] = '\0';

	}
	else {

		// Tokenize based on space
		char *command = strtok(buffer, " ");

		if (strcmp(command, "/msg") == 0) {

			msg->command_type = MSG;
			char *user = strtok(NULL, " ");
			char *text = strtok(NULL, "");
			if (user) {
			       
				strncpy(msg->user_name, user, USER_NAME - 1);
				msg->user_name[USER_NAME - 1] = '\0';
			
			}
			if (text) {
			       
				strncpy(msg->data, text, DATA_SIZE - 1);
				msg->data[DATA_SIZE - 1] = '\0';
			
			}

		}
		else if (strcmp(command, "/file") == 0) {

			msg->command_type = FILE_SEND;
			char *user = strtok(NULL, " ");
			char *filename = strtok(NULL, "");
			if (user) {
			       
				strncpy(msg->user_name, user, USER_NAME - 1);
				msg->user_name[USER_NAME -1] = '\0';

			}
			if (filename) {
				       
				strncpy(msg->file_name, filename, FILE_NAME - 1);
				msg->file_name[FILE_NAME - 1] = '\0';
			}

		}
		else if (strcmp(command, "/list") == 0) {

			msg->command_type = LIST;

		}
		else if (strcmp(command, "/quit") == 0) {

			msg->command_type = QUIT;

		}
		else {

			msg->command_type = 0;

		}
	
	}

}


int main() {

	const char *test_inputs[] = {
		"Hello Every One!",
		"/msg bob hello bob",
		"/file bob tcp_udp_guideline.pdf",
		"/list",
		"/msg arpit Hello arpit what are you doing?",
		"Welcome to EIC",
		"/quit"
	};

	client_msg msg;

	int no_of_test_cases = sizeof(test_inputs)/sizeof(test_inputs[0]);

	for (int i = 0; i < no_of_test_cases; ++i) {

		printf("Input: %s\n", test_inputs[i]);
		input_parser(test_inputs[i], &msg);

		switch (msg.command_type) {

			case BROADCAST: 
				printf("Broadcast message: \"%s\"\n", msg.data);
				break;

			case MSG: 
				printf("Message: \"%s\" sent to user: %s\n", msg.data, msg.user_name);
				break;

			case FILE_SEND: 
				printf("File: \"%s\" sent to user: %s\n", msg.file_name, msg.user_name);
				break;

			case LIST: 
				printf("List all current clients\n");
				break;

			case QUIT: 
				printf("Quit command received, exiting\n");
				break;

		}

		printf("\n");
	
	}	

	return 0;

}
