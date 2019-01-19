#include <errno.h>
#include "SocketUtils.h"

/**
 * Authors: Paul TORTEL & Vincent COURSAC
 * Promo 2019 - Software Engineering
 * 
 * Useful functions for socket managing (close socket function, sending message function)
 *  
 */
 
int closeSocket(int* socket) {
    return close(*socket);
}

void sendMessage(socket_msg* msg, int clientSocket) {
    char* message = prepareMsg(msg);
    
    if(send(clientSocket, message, BUFFER_SIZE, 0) < 0) {
        perror("send()");
        exit(errno);
    }
	
	free(message);
}


int equals(char *str1, char* str2) {
	for (int i = 0;; i++)
    {
        if (str1[i] == '\0' && str2[i] == '\0')
        {
            break;
        }
		else if (str1[i] == '\0' && str2[i] == '$')
        {
            break;
        }
		else if (str1[i] == '$' && str2[i] == '\0')
        {
            break;
        }
		else if (str1[i] == '$' && str2[i] == '$')
        {
            break;
        }
        else if (str1[i] == '\0' && str2[i] != '\0')
        {
            return 0;
        }
        else if (str1[i] != '\0' && str2[i] == '\0')
        {
            return 0;
        }
		else if (str1[i] == '$' && str2[i] != '$')
        {
            return 0;
        }
        else if (str1[i] != '$' && str2[i] == '$')
        {
            return 0;
        }
        else if (str1[i] != str2[i])
        {
            return 0;
        }
    }
    return 1;
}
