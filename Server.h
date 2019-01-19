#ifndef SERVER_FILE
#define SERVER_FILE

#include "game.h"


/**
 * Authors: Paul TORTEL & Vincent COURSAC
 * Promo 2019 - Software Engineering
 * 
 * Create a new server state, verification function when someone attack a position
 * handle of the message received by the server
 * 
 */
 
typedef struct {
	int clients[2];
	GridState grids[2][10][10];
	int running;
	int state;
	int turn;
	sem_t *clientReady[2];
  
} ServerState;

ServerState* newServerState();

void handleServerMessage(ServerState *serverstate, int playerID, char* raw);

void requestPlay(ServerState *serverstate);

int checkShips(ServerState* serverstate, int attacked);
#endif