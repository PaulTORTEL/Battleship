/**
 * Authors: Paul TORTEL & Vincent COURSAC
 * Promo 2019 - Software Engineering
 * 
 * Create a new server state, verification function when someone attack a position
 * handle of the message received by the server
 * 
 */

#include "Server.h"
#include "SocketUtils.h"
#include "semaphore.h"
#include <sys/mman.h>
#include <stdlib.h>
#include <stdio.h>

ServerState* newServerState() {
	ServerState *state = malloc(sizeof(ServerState));
	
	for (int i = 0; i < 2; i++) {
        state->clientReady[i] = (sem_t*) malloc(sizeof(sem_t));
		sem_init(state->clientReady[i], 0, 0);
	}
	
	state->running = 1;
	
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			for (int k = 0; k < 2; k++) {
				state->grids[k][i][j].type = 0;
				state->grids[k][i][j].touched = 0;
			}
		}
	}
	
	return state;
}


void handleServerMessage(ServerState *serverstate, int playerID, char* raw) {
	socket_msg* msg = parseMsg(raw);
	
	char* channel = msg->channel;
	
	if (equals(channel, "set_ships")) {
		
		
		for (int i = 0; i < 30; i++) {
			char* arg = msg->arguments[i];
			int count = 0;
			char **split = splitSring(arg,'-',&count);
			
			if (count != 3) {
				printf("Error, 3 arguments are required to define a ship location\n");
			}
			
			int type = atoi(split[0]);
			int x = atoi(split[1]);
			int y = atoi(split[2]);
			
			serverstate->grids[playerID][x][y].type = type;
		}
		
		sem_post(serverstate->clientReady[playerID]);
	}
	else if (equals(channel, "attack")) {

		int attacked = 1 - playerID;
		if (playerID != serverstate->turn) return;
		
		
		int x = atoi(msg->arguments[0]);
		int y = atoi(msg->arguments[1]);
		

		
		if (serverstate->grids[attacked][x][y].touched == 1) {
			requestPlay(serverstate);
		}
		else {

			serverstate->grids[attacked][x][y].touched = 1;
			
			int touched = (serverstate->grids[attacked][x][y].type > 1);
			
			if (serverstate->grids[attacked][x][y].type == 0)
				serverstate->grids[attacked][x][y].type = 1;
			
            
            socket_msg update; 
            update.channel = "update";
            char *arguments[5];
            char winner[3];
            
            // Check if a player has been defeated
			if (checkShips(serverstate, attacked) == 0) {
                sprintf(winner, "%d", serverstate->turn);
                winner[1] = '\0';
                serverstate->running = 0;
            }
            
            else {
                sprintf(winner, "-1");
                winner[2] = '\0';
            }
                
            serverstate->turn = attacked;
            
            char xS[2];
            char yS[2];
            char tS[2];
            char to[2];
            
            sprintf(xS, "%d", x);
            xS[1] = '\0';
            
            sprintf(yS, "%d", y);
            yS[1] = '\0';
            
            sprintf(tS, "%d", attacked);
            tS[1] = '\0';
            
            sprintf(to, "%d", touched);
            to[1] = '\0';
            
            
            arguments[0] = xS;
            arguments[1] = yS;
            arguments[2] = tS;
            arguments[3] = to;
            arguments[4] = winner;
            
            update.arguments = arguments;
            update.count = 5;
            
            sendMessage(&update, serverstate->clients[0]);
            sendMessage(&update, serverstate->clients[1]);
		}
	}

	freeMsg(msg);
}

void requestPlay(ServerState *serverstate) {
	socket_msg play; 
	play.channel = "play";
	char *arguments[1];
	char pID[2];
	sprintf(pID, "%d", serverstate->turn);
	pID[1] = '\0';
	arguments[0] = pID;
	play.arguments = arguments;
	play.count = 1;
	sendMessage(&play, serverstate->clients[0]);
	sendMessage(&play, serverstate->clients[1]);
}


int checkShips(ServerState* serverstate, int attacked) {
        
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            if (serverstate->grids[attacked][i][j].touched == 0
                && serverstate->grids[attacked][i][j].type != 0)
                return 1;
        }
    }
    return 0;
}