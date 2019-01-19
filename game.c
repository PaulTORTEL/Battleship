/**
 * Authors: Paul TORTEL & Vincent COURSAC
 * Promo 2019 - Software Engineering
 * 
 * Functions for the game : game state structure to keep track of the progression of the game
 * update board function called when a position has been shot
 * input ships functions called when players are ready to play
 * not used anymore functions to place by default ships
 */

#include "game.h"
#include "Input.h"
#include <sys/mman.h>
#include <stdlib.h>
#include <stdio.h>

GameState* newGameState() {
	GameState *state = (GameState*) malloc(sizeof(GameState));
	state->running = 1;
	
    state->semaphore = (sem_t*) malloc(sizeof(sem_t));
	sem_init(state->semaphore, 0, 0);
	    
    state->justTouched = -1;
    state->winner = -1;
	return state;
}

void updateBoard(GameState* gamestate, int x, int y, int boardID, int touched) {
	
	if (gamestate->grids[boardID][x][y].type == 0) {
		gamestate->grids[boardID][x][y].type = 1;
	}
	gamestate->grids[boardID][x][y].touched = touched;	
}

void inputShips(GameState* gamestate) {
	
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			for (int k = 0; k < 2; k++) {
				gamestate->grids[k][i][j].type = 0;
				gamestate->grids[k][i][j].touched = 0;
			}
		}
	}

	int shipsLeft = 10;
	
	int left1 = 1;
	int left2 = 2;
	int left3 = 3;
	int left4 = 4;
	
	int error = 0;
    
		
	while (shipsLeft != 0) {
		printf("\033[H\033[J");
		printf("== Input your ships ==\n\n");
		DisplayBoard(gamestate, gamestate->playerID);
		printf("Carrier : %d    (Size : 5)  |  Battleships : %d (Size : 4)          \n", left1, left2);
		printf("Submarines : %d (Size : 3)  |  Destroyers : %d (Size : 2)          \n", left3, left4);
		
		if (error) {
			error = 0;
			printf("\n Your input was invalid\n");
		}
		
		printf("\n-- First coordinate (XY ~ Example : A5 ) --\n");
		
		int firstX = -1;
		int firstY = -1;
		
		if (!inputCoordinate(&firstX, &firstY)) {
			error = 1;
			continue;
		}
		
		printf("\n-- Second coordinate (XY ~ Example : A5 ) --\n");
		
		int secondX = -1;
		int secondY = -1;
		
		if (!inputCoordinate(&secondX, &secondY)) {
			error = 1;
			continue;
		}
			
		if (firstX > secondX || firstY > secondY) {

			error = 1;
			continue;
		}
		
		if (firstX == secondX && firstY == secondY) {
			error = 1;
			continue;
		}
		
		if (firstX != secondX && firstY != secondY) {
			error = 1;
			continue;
		}
		
		int delta = 0;
		int len = 1;
		if (firstX == secondX) {
			delta = 1;
			len += secondY - firstY;
		}
		else {
			len += secondX - firstX;
		}
		
		if (len < 2 || len > 5) {
			error = 1;
			continue;
		}
		
		if (len == 5 && left1 == 0) {
			error = 1;
			continue;
		}
		
		if (len == 4 && left2 == 0) {
			error = 1;
			continue;
		}
		
		if (len == 3 && left3 == 0) {
			error = 1;
			continue;
		}
		if (len == 2 && left4 == 0) {
			error = 1;
			continue;
		}
		
		for (int i = 0; i < len; i++) {
			int x = firstX + (i * (1 - delta));
			int y = firstY + (i * delta);
			
			if (gamestate->grids[gamestate->playerID][x][y].type != 0) {
				error = 1;
				break;
			}
		
		}
		
		if (error) {
			continue;
		}
		
		for (int i = 0; i < len; i++) {
			int x = firstX + (i * (1 - delta));
			int y = firstY + (i * delta);
			
			gamestate->grids[gamestate->playerID][x][y].type = 2 + (5 - len);
		
		}
		
		if (len == 5) left1 -= 1;
		if (len == 4) left2 -= 1;
		if (len == 3) left3 -= 1;
		if (len == 2) left4 -= 1;
		
		shipsLeft -= 1;
		
		printf("\033[H\033[J");
		
	}
	
	printf("\033[H\033[J");
	
}

void fillDefaultShips(GameState* gamestate) {
	int ID = gamestate->playerID;
	
	for (int i = 0; i < 5; i++) {
		gamestate->grids[ID][0][i].type = 2;
	}
    
	for (int i = 0; i < 4; i++) {
		gamestate->grids[ID][2][i].type = 3;
	}
	
	for (int i = 0; i < 4; i++) {
		gamestate->grids[ID][4][i].type = 3;
	}
	
	for (int i = 0; i < 3; i++) {
		gamestate->grids[ID][6][i].type = 4;
	}
	
	for (int i = 0; i < 3; i++) {
		gamestate->grids[ID][8][i].type = 4;
	}
	
	for (int i = 0; i < 3; i++) {
		gamestate->grids[ID][0][i + 6].type = 4;
	}
	
	for (int i = 0; i < 2; i++) {
		gamestate->grids[ID][2][i + 6].type = 5;
	}
	
	for (int i = 0; i < 2; i++) {
		gamestate->grids[ID][4][i + 6].type = 5;
	}
	
	for (int i = 0; i < 2; i++) {
		gamestate->grids[ID][6][i + 6].type = 5;
	}
	
	for (int i = 0; i < 2; i++) {
		gamestate->grids[ID][8][i + 6].type = 5;
	}
}