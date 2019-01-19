#ifndef CLIENTPREPARATION_FILE
#define CLIENTPREPARATION_FILE

#include "ServerUtils.h"
#include <semaphore.h>


/**
 * Authors: Paul TORTEL & Vincent COURSAC
 * Promo 2019 - Software Engineering
 * 
 * Functions to create the client (handle connection to the server)
 * Start function for the listening thread, call of the launch function for the game
 * 
 */
 
ServerInfo prepareClient();

// Connect the client (the one who forked) and return his socket
int connectClient(ServerInfo* infos, sem_t* sem);

// Listening thread foreach clients
void* listeningThreadClient(void* arg);

// Manage client game: connection, etc
void manageClient(ServerInfo* infos, sem_t* sem);

#endif