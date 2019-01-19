/**
 * Authors: Paul TORTEL & Vincent COURSAC
 * Promo 2019 - Software Engineering
 * 
 * Game loop function, function to handle the message received,
 * handle play turn (selection of where the player wants to attack)
 * handle ship creation on client side
 * 
 */

#include "Client.h"
#include "SocketUtils.h"
#include "semaphore.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "Input.h"


void launchGame(GameState* gamestate) {
	clearConsole();
	int state = -1;
	
	while (gamestate->running) {
		clearConsole();
		
		state = gamestate->state;
		
		if (state == 0) {
			printf("Waiting for server...\n");
		}
		else if (state == 1) {
			printf("Waiting for another player...\n");
		}
		else if (state == 2) {
			handleShipCreation(gamestate);
		}
		else if (state == 3) {
			handlePlayTurn(gamestate);
		}
		else if (state == 4) {
			DisplayBoards(gamestate);
			printf("Your opponent is attacking...\n");
		}
        else if (state == 5) {
            DisplayBoards(gamestate);
            printf("End of the game. Thanks for playing!\n");
            gamestate->running = 0;
            return;
        }
        else if (state == 6) {
            DisplayBoards(gamestate);
            printf("Connection lost with the server...\nEnd of the game.\n\n");
            gamestate->running = 0;
            return;
        }
		sem_wait(gamestate->semaphore);
	}
	
}

void handleMessage(GameState* gamestate, char* raw) {
	
	socket_msg* msg = parseMsg(raw);
	
	char* channel = msg->channel;
	
	if (equals(channel, "welcome")) {
		int playerID = atoi(msg->arguments[0]);
		gamestate->playerID = playerID;
		gamestate->state = 1;
		sem_post(gamestate->semaphore);
	}

	else if (equals(channel,"ready")) {
		gamestate->state = 2;
		sem_post(gamestate->semaphore);
	}
	else if (equals(channel,"play")) {
		int playerID = atoi(msg->arguments[0]);
		
		if (playerID == gamestate->playerID) 
            gamestate->state = 3;
		else
            gamestate->state = 4;
		
		sem_post(gamestate->semaphore);
	}
    
    else if (equals(channel, "disconnection")) {
        gamestate->state = 6;
        socket_msg msg;
        msg.channel = "";
        msg.count = 0;
        
        sendMessage(&msg, gamestate->client);
        sem_post(gamestate->semaphore);
    }
	
	else if (equals(channel,"update")) {
		
		int x = atoi(msg->arguments[0]);
		int y = atoi(msg->arguments[1]);
		int turn = atoi(msg->arguments[2]);
		int touched = atoi(msg->arguments[3]);
        int winnerID = atoi(msg->arguments[4]);
		
		if (turn == gamestate->playerID) {
			gamestate->state = 3;
			
			gamestate->grids[gamestate->playerID][x][y].touched = touched;
            
            if (touched)
                gamestate->justTouched = 2;
            
            else
                gamestate->justTouched = 3;
			
            if (gamestate->grids[gamestate->playerID][x][y].type == 0) {
				gamestate->grids[gamestate->playerID][x][y].type = 1;
			}
		}
		else {
			gamestate->state = 4;
			
			gamestate->grids[turn][x][y].touched = touched;
			gamestate->justTouched = touched;
            
			if (gamestate->grids[turn][x][y].type == 0) {
				gamestate->grids[turn][x][y].type = 1;
			}
		}
        
        if (winnerID == gamestate->playerID) {
            gamestate->winner = 1;
            gamestate->state = 5;
        }
        else if (winnerID != -1) {
            gamestate->winner = 0;
            gamestate->state = 5;
        }
		
		sem_post(gamestate->semaphore);
	}
	
	freeMsg(msg);
	
	usleep(1000);
}

void handlePlayTurn(GameState* gamestate) {
	
	
	int ID = 1 - gamestate->playerID;
	
	int correct = 0;
	
	int x = -1;
	int y = -1;
	int error = 0;
	
	while (!correct) {
		
		clearConsole();
		
		DisplayBoards(gamestate);
		
		printf("Your turn to attack, input a position (Example : A5)\n");
		
		if (error) {
			error = 0;
			printf("Your input was invalid\n");
		}
		
		if (!inputCoordinate(&x, &y)) {
			error = 1;
			continue;
		}
		
		if (gamestate->grids[ID][x][y].type != 0) {
			error = 1;
			continue;
		}
		
		correct = 1;
	}
	
	socket_msg attack; 
	attack.channel = "attack";
	char *arguments[2];
	char xS[2];
	char yS[2];
	sprintf(xS, "%d", x);
    xS[1] = '\0';
    
	sprintf(yS, "%d", y);
    yS[1] = '\0';
    
	arguments[0] = xS;
	arguments[1] = yS;
	attack.arguments = arguments;
	attack.count = 2;
	sendMessage(&attack, gamestate->client);
	
	printf("\nWaiting for server...\n");
}

void handleShipCreation(GameState* gamestate) {
	inputShips(gamestate);
	clearConsole();
	
	socket_msg setShips; 
	setShips.channel = "set_ships";
	char *arguments[30];
	int c = 0;
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			if (gamestate->grids[gamestate->playerID][i][j].type > 1) {
				arguments[c] = malloc(10 * sizeof(char));
				sprintf(arguments[c], "%d-%d-%d\0", gamestate->grids[gamestate->playerID][i][j].type, i,j);
				c++;
			}
		}
	}
	
	setShips.arguments = arguments;
	setShips.count = 30;
	
	sendMessage(&setShips, gamestate->client);

	clearConsole();
	
	printf("     Your board    \n");
	DisplayBoard(gamestate, gamestate->playerID);
	
	printf("\nWaiting for the other player...\n");
}