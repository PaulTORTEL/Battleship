/**
 * Authors: Paul TORTEL & Vincent COURSAC
 * Promo 2019 - Software Engineering
 * 
 * Useful functions to handle strings (arguments)
 * Split strings, parsing of structure, building of a structure from a string...
 *  
 */

#include "utils.h"
#include "SocketUtils.h"
#include "stdio.h"

char** splitSring(char *str, char separator, int *result) {
	char** splitted; //the array of string which will  be returned
	int count = 0;
	int len = strlen(str);
	for (int i = 0; i < len; i++) {  //we start to count the occurences of the separator in the original string
		if (str[i] == separator && i != 0 && i != (len - 1)) {
			count++;
		}
		
	}
	count++;// we add one because for one separator, there are two substrings : example = "hello%sir", => one occurency of '%', two substrings
	splitted = (char**) malloc(count * sizeof(char*));
	int current = 0;// the current substring
	int begin = 0;//the last position of a separator
	int end = 0;//the current position which is not a separator
	for (int i = 0; i < strlen(str); i++) { //we analyze the string
		if (str[i] == separator) { // if we  find a separator...
			
			if (i != 0) { // ... and it is not a the begining of the string ...
			
				char* temp = (char*) malloc(((end - begin) + 1) * sizeof(char)); //we create a substring
					int c = 0;
				for (int k = begin; k < end; k++) { //and we fill it with the characters before the separator
					temp[c] = str[k];
					c++;
				}
				temp[c] = '\0';
				
				splitted[current] = temp; //we add the substring to the array of substrings
				current++; //and we increase the current substring count
			}
			
			begin = end + 1; //we update the begin
			
		}
		end++; 
	}
	if (begin != (len)) { //at the end, if the last position of the begin is not the end, we have another substring to add
		int c = 0;
		char* temp = (char*) malloc(((end - begin) + 1) * sizeof(char));
		for (int k = begin; k < end; k++) {
			temp[c] = str[k];
			c++;
		}
		splitted[current] = temp;
	}

	*result = count; //we put the amount of substrings in the ouput result count
	return splitted;// and return the array of subtrings
}

socket_msg* parseMsg(char* str) { //same logic as the split, except we consider the first substring as the channel, the others are the arguments
	
	char separator = '$'; //and we set the separator as '$'
	socket_msg *msg = malloc(sizeof(socket_msg));
	char** splitted;
	int count = 0;
	int len = 0; //the real length of the message
	for (int i = 0; i < BUFFER_SIZE; i++) {
		if (str[i] == '~') {
			len = i;
			break;
		}
	}
	for (int i = 0; i < len; i++) {
		if (str[i] == separator && i != 0 && i != (len - 1)) {
			count++;
		}
	}
	//we don't add the count++ because the first substring will not be counted
	
	splitted = (char**) malloc(count * sizeof(char*));
	int current = -1; //we set the current to -1 for the same reason
	int begin = 0;
	int end = 0;
	for (int i = 0; i < len; i++) {
		if (str[i] == separator) {
			
			if (i != 0) {
				int allocated = (end - begin) + 1;
				
				char* temp = (char*) malloc(allocated * sizeof(char));
					int c = 0;
				for (int k = begin; k < end; k++) {
					temp[c] = str[k];
					c++;
				}
				temp[c] = '\0';
				if (current == -1) //if it is the first substring, we set it as the message channel
				{
					msg->channel = temp;
				}
					
				else //if not, we add the substrings to the arguments
					splitted[current] = temp;
				
				current++;
			}
			
			begin = end + 1;
			
		}
		end++;
	}
	if (begin != (len)) {
		int c = 0;
		char* temp = (char*) malloc(((end - begin) + 1) * sizeof(char));
		for (int k = begin; k < end; k++) {
			temp[c] = str[k];
			c++;
		}
		splitted[current] = temp;
	}
	msg->arguments = splitted;
	msg->count = count;
	return msg;
}

char* prepareMsg(socket_msg *msg) {
	// we first have to count the final length of the ouput string
	
	int len = strlen(msg->channel) + 1; //we add to the length the size of the channel + 1 for the separator 
	
	if (msg->count > 0) //if we have arguments
		len += (msg->count - 1); // we add the number of arguments less 1 (1 separator = two arguments)
	
	for (int i = 0; i < msg->count; i++) {
		len+= strlen(msg->arguments[i]); //then we add the length of each arguments
	}
	
	char* str = malloc(BUFFER_SIZE * sizeof(char)); //we create the string
	int c = 0;
	for (int i = 0; i < strlen(msg->channel); i++) {
		
		str[c] = msg->channel[i]; //we fill it with the channel
		c++;
	}
	str[c] = '$'; //then the first separator
	c++;
	
	for (int i = 0; i < msg->count; i++) {
		for (int k = 0; k < strlen(msg->arguments[i]) ; k++) {
			str[c] = msg->arguments[i][k]; //and the arguments ...
			c++;
		}
		if (i < msg->count - 1) {
			str[c] = '$'; // ... separated with separator
			c++;
		}	
		
	}
	str[c] = '~';
	for (int i = (c+1); i < BUFFER_SIZE; i++) {
		str[i] = '|';
	}
	
	return str;
}

void freeMsg(socket_msg *msg) {
	free(msg->channel);
	for (int i = 0; i < msg->count; i++) {
		free(msg->arguments[i]);
	}
	free(msg->arguments);
	free(msg);
}

void freeDString(char** str, int count) {
	for (int i = 0; i < count; i++) {
		free(str[i]);
	}
	free(str);
}

void clearConsole() {
    
    if (DEBUG == 0) {
        printf("\033[H\033[J");
        fflush(stdout);
    }
}