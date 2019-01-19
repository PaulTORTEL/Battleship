#ifndef SOCKETUTILS_FILE
#define SOCKETUTILS_FILE

#define ERROR_SOCKET -1
#define BUFFER_SIZE 200
#define DEBUG 0

#include "utils.h"

/**
 * Authors: Paul TORTEL & Vincent COURSAC
 * Promo 2019 - Software Engineering
 * 
 * Useful functions for socket managing (close socket function, sending message function)
 *  
 */

// Close the socket sent
int closeSocket(int* socket);

// Send a message
void sendMessage(socket_msg* msg, int clientSocket);


int equals(char *str1, char* str2);

#endif