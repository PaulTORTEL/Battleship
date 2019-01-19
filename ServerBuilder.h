#ifndef SERVERPREPARATION_FILE
#define SERVERPREPARATION_FILE

#include "ServerUtils.h"
#include <semaphore.h>
#include "Server.h"


/**
 * Authors: Paul TORTEL & Vincent COURSAC
 * Promo 2019 - Software Engineering
 * 
 * Functions for preparation of the server (listen(), accept() for connection, starting of listening threads...)
 * 
 */

// Ask the user to input the informations about the server he wants to create
ServerInfo prepareServer();

// Create and bind the server socket
int setupServerSocket(ServerInfo* infos);

// Connect clients (wait till there is two players)
void connectClients(int serverSocket, sem_t* sem, ServerState* serverstate);

// Listening thread for each connected client
void* listeningThread(void* arg);

// Launch the server in the child process
void launchServer(ServerInfo *infos, sem_t* sem);

// Notify that the remote player has disconnected
void notifyLocalClientDisconnection(int clientSocket);

#endif