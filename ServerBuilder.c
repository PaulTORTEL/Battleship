/**
 * Authors: Paul TORTEL & Vincent COURSAC
 * Promo 2019 - Software Engineering
 * 
 * Functions for preparation of the server (listen(), accept() for connection, starting of listening threads...)
 * 
 */

#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include "ServerBuilder.h"
#include "Input.h"
#include "SocketUtils.h"
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

ServerInfo prepareServer() {
    
    ServerInfo infos;
    
	printf("\n\nInput the server PORT");
	
	infos.IP = "localhost";
    infos.PORT = inputRangeInt(1000,99999);
    
    return infos;
}

// Create and bind the server socket
int setupServerSocket(ServerInfo* infos) {
    
    // Server socket creation
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    
    // If there is an error, serverSocket = -1
    if(serverSocket == ERROR_SOCKET) {
        perror("socket()");
        exit(errno);
    }
    
    struct sockaddr_in serverAddress = { 0 };
    
    // We accept any address because it is the server
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(infos->PORT);

    // Binding of the serverSocket with the structure sockaddr_in
    if(bind(serverSocket, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) == ERROR_SOCKET) {
        perror("bind()");
        exit(errno);
    }
    
    return serverSocket;
}

// Connect clients (wait till there is two players)
void connectClients(int serverSocket, sem_t* sem, ServerState* serverstate) {
    
    // 2 simultaneous connections max.
    // serverSocket will be able to listen for connections
    if(listen(serverSocket, 2) == ERROR_SOCKET) {
        perror("listen()");
        exit(errno);
    }
    
    struct sockaddr_in clientAddress = { 0 };
    socklen_t clientAddrSize = sizeof clientAddress;
    
    //Notify the local client to connect
    sem_post(sem);
        
    // We want to accept 2 clients
    for (int i = 0; i < 2; i++) {
        
        int clientSocket;        
            
        clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddrSize);

        if(clientSocket == ERROR_SOCKET) {
            perror("accept()");
            exit(errno);
        }
		
        if (DEBUG)
        printf("\n[SERVER] A client has connected!\n");
        
		serverstate->clients[i] = clientSocket;
        
		socket_msg welcome; 
		welcome.channel = "welcome";
		char *arguments[1];
		char pID[2];
		sprintf(pID, "%d", i);
		pID[1] = '\0';
		arguments[0] = pID;
		welcome.arguments = arguments;
		welcome.count = 1;
		sendMessage(&welcome, clientSocket);
    }

	socket_msg ready; 
	ready.channel = "ready";
	ready.count = 0;
	sendMessage(&ready, serverstate->clients[0]);
	sendMessage(&ready, serverstate->clients[1]);
	serverstate->state = 1;
}

void* listeningThread(void* arg) {
    
    ListenServerThreadArgs* listArgs = arg;
    
	ServerState *serverstate = listArgs->serverstate;
	
	int playerID = listArgs->playerID;
	
    int socket = serverstate->clients[playerID];

    while (serverstate->running) {
        
        char messageReceived[BUFFER_SIZE];
        ssize_t size = recv(socket, messageReceived, sizeof(messageReceived), 0);
		
        if (size <= 0 || serverstate->running == 0) {
            serverstate->running = 0;
            return NULL;
        }
        
		if (DEBUG)
        printf("\n[SERVER] Message received:\n%s [size = %ld]\n", messageReceived, size);

        // Call of the function to handle the message
        handleServerMessage(serverstate, playerID, messageReceived);
    }
    
    return NULL;
}

void launchServer(ServerInfo *infos, sem_t* sem) {
	
	
    int serverSocket = setupServerSocket(infos);
	
	ServerState *serverstate = newServerState();
	
   
    // Connect clients
	connectClients(serverSocket, sem, serverstate);
	
	if (DEBUG)
    printf("\n[DEBUG][SERVER] Everyone is connected, server can start listening to clients\n");
    
    // Args for the listening threads
    ListenServerThreadArgs listArgs;
	listArgs.serverstate = serverstate;
	listArgs.playerID = 0;
	
    
    ListenServerThreadArgs listArgs2;
	listArgs2.serverstate = serverstate;
	listArgs2.playerID = 1;

    
    // Listening threads declaration    
    pthread_t client1ListThread;
    pthread_t client2ListThread;
    
    // Create and run the threads
    // Listening thread for client 1
    if(pthread_create(&client1ListThread, NULL, listeningThread, (void*)&listArgs) == -1) {
        perror("pthread_create");
        return;
    }
    // Listening thread for client 2
    if(pthread_create(&client2ListThread, NULL, listeningThread, (void*)&listArgs2) == -1) {
        perror("pthread_create");
        return;
    }
    
	for (int i = 0; i < 2; i++) {
		sem_wait(serverstate->clientReady[i]); //while the two players haven't created their ships
	}
	
	serverstate->state = 1;
    
    srand(time(NULL));
	serverstate->turn = rand()%2; // The turn is randomized
	
	requestPlay(serverstate);
		
	
    pthread_join(client2ListThread, NULL);
    usleep(1);
    notifyLocalClientDisconnection(serverstate->clients[0]);
    
    pthread_join(client1ListThread, NULL);
    
    
    free(serverstate->clientReady[0]);
    free(serverstate->clientReady[1]);
    
    closeSocket(&serverSocket);
    closeSocket(&(serverstate->clients[0]));
	
	closeSocket(&(serverstate->clients[1]));
    free(serverstate);
}

void notifyLocalClientDisconnection(int clientSocket) {
    
    socket_msg msg;
    msg.channel = "disconnection";
    msg.count = 0;
    
    sendMessage(&msg, clientSocket);
}