/**
 * Authors: Paul TORTEL & Vincent COURSAC
 * Promo 2019 - Software Engineering
 * 
 * Functions to create the client (handle connection to the server)
 * Start function for the listening thread, call of the launch function for the game
 * 
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include "ClientBuilder.h" 
#include "Input.h"
#include "SocketUtils.h"
#include "Client.h"

ServerInfo prepareClient() {
	ServerInfo infos;
	
	printf("\nInput the server IP you want to join\n");
	infos.IP = inputString();
	printf("\nInput the server PORT you want to join");
	infos.PORT = inputRangeInt(1000,99999);
	
    return infos;
}

// Connect local client and return his socket
int connectClient(ServerInfo* infos, sem_t* sem) {
    
    // Create the client socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    if(clientSocket == ERROR_SOCKET)
    {
        perror("socket()");
        exit(errno);
    }
    
    struct hostent *hostInfo = NULL;
    struct sockaddr_in clientAddress = { 0 };
    
    // Set up of the hostname
    const char *hostname = infos->IP;

    hostInfo = gethostbyname(hostname);

    // If the host doesn't exist or could not be reached for some reasons
    if (hostInfo == NULL) {
        fprintf (stderr, "Unknown host %s.\n", hostname);
        exit(EXIT_FAILURE);
    }

    // Fill of the structure which contains every details about the connection that need to be made
    clientAddress.sin_addr = *(struct in_addr *) hostInfo->h_addr;
    clientAddress.sin_port = htons(infos->PORT);
    clientAddress.sin_family = AF_INET;

    // Wait for the local server to be ready to accept connections before attempting to connect
    if (sem != NULL)
        sem_wait(sem);

    // Try to connect to the server
    if(connect(clientSocket,(struct sockaddr *) &clientAddress, sizeof(struct sockaddr)) == ERROR_SOCKET) {
        perror("connect()");
        exit(errno);
    }
    
    return clientSocket;
}

void* listeningThreadClient(void* arg) {
    
    GameState *gamestate = arg;
	
    int socket = gamestate->client;

    while (gamestate->running) {
        char messageReceived[BUFFER_SIZE];
        ssize_t size = recv(socket, messageReceived, sizeof(messageReceived), 0);
		
        if (size <= 0) {
            gamestate->state = 6;
            sem_post(gamestate->semaphore);
            pthread_exit();
        }
		if (DEBUG)
        printf("\n[CLIENT]Message received:\n%s [size = %d]\n", messageReceived, size);
        
        // Call of the function to handle the message
        handleMessage(gamestate, messageReceived);
    }
    
    return NULL;
}

void manageClient(ServerInfo* infos, sem_t* sem) {
    
    int clientSocket = connectClient(infos, sem);  
    
	GameState *gamestate = newGameState();
	gamestate->client = clientSocket;
    
    pthread_t listeningThread;
    if(pthread_create(&listeningThread, NULL, listeningThreadClient, (void*)gamestate) == -1) {
        perror("pthread_create");
        return EXIT_FAILURE;
    }
     
	
	launchGame(gamestate);
	
    free(gamestate->semaphore);
    
    closeSocket(&clientSocket);
   
}
