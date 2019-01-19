#ifndef UTILS_FILE
#define UTILS_FILE

/**
 * Authors: Paul TORTEL & Vincent COURSAC
 * Promo 2019 - Software Engineering
 * 
 * Useful functions to handle strings (arguments)
 * Split strings, parsing of structure, building of a structure from a string...
 *  
 */

//struct use to manage messages over the sockets
typedef struct {
	char* channel; //channel describes the content of the message (example : update_coordinate)
	char** arguments; //the arguments are the substrings sent with the message
	int count; //the ammount of arguments
} socket_msg;

//Split a string in an array of string using the given separator ===> hello%sir -> split with '%' --> { "hello", "sir" }
char** splitSring(char *str, char separator, int *result);

//create a socket message struct from a raw string, used for manage an incoming message from a socket
socket_msg* parseMsg(char* str);

//return a string parsed from a socket message, the string is ready to be sent over a socket
char* prepareMsg(socket_msg *msg);

//free a socket message
void freeMsg(socket_msg *msg);

//free an array of string, for example the result of a split
void freeDString(char** str, int count);

void clearConsole();

#endif

/**

usage of socket messages : 
 
  	socket_msg output; 
	output.channel = "test";
	char *arguments[2];
	arguments[0] = "first";
	arguments[1] = "second";
	output.arguments = arguments;
	output.count = 2;
	
	char* str = prepareMsg(&output);
	printf("String message : %s\n", str);
	

	
	socket_msg* input = parseMsg(str);
	
	printf("\n\nReceived message : \n");
	printf("Channel : %s\n", input->channel);
	printf("Count : %d\n", input->count);
	for (int i = 0; i < input->count; i++) {
		printf("Argument[%d] = %s\n", i, input->arguments[i]);
	}
	
	freeMsg(input);
	free(str);

**/