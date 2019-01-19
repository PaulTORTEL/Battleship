#ifndef SERVERUTILS_FILE
#define SERVERUTILS_FILE

#include "Server.h"

/**
 * Authors: Paul TORTEL & Vincent COURSAC
 * Promo 2019 - Software Engineering
 * 
 * Structures for encapsulate server informations and arguments to pass to the functions of the listening threads
 * 
 */
 
typedef struct {
    char* IP;
    int PORT;
} ServerInfo;

typedef struct {
	ServerState *serverstate;
    int playerID;
} ListenServerThreadArgs;


#endif