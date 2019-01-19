/**
 * Authors: Paul TORTEL & Vincent COURSAC
 * Promo 2019 - Software Engineering
 * 
 * Function to display player's board and function to print depending on which type is a cell (FR: case)
 * 
 */
 
#include "game.h"
#include <stdio.h>

void printCell(GridState *state) {
	if (state->type == 0) printf("-");
	
	else if (state->type == 1) {
		if (state->touched == 0) {
            printf("\033[%sm", "36");
            printf("o");
            printf("\033[%sm", "0");
        }
		else {
            printf("\033[%sm", "31");
            printf("x");
            printf("\033[%sm", "0");
        }
    }
		
	else if (state->type == 2) {
		if (state->touched == 0) {
            printf("\033[%sm", "32");
            printf("C");
            printf("\033[%sm", "0");
        }
		else {
            printf("\033[%sm", "31");
            printf("c");
            printf("\033[%sm", "0");
        }
    }
		
	else if (state->type == 3) {
		if (state->touched == 0) {
            printf("\033[%sm", "32");
            printf("B");
            printf("\033[%sm", "0");
        }
		else {
            printf("\033[%sm", "31");
            printf("b");
            printf("\033[%sm", "0");
        }
    }
		
	else if (state->type == 4) {
		if (state->touched == 0) {
            printf("\033[%sm", "32");
            printf("S");
            printf("\033[%sm", "0");
        }
		else {
            printf("\033[%sm", "31");
            printf("s");
            printf("\033[%sm", "0");
        }
    }
		
	else if (state->type == 5) {
		if (state->touched == 0) {
            printf("\033[%sm", "32");
            printf("D");
            printf("\033[%sm", "0");
        }
		else {
            printf("\033[%sm", "31");
            printf("d");
            printf("\033[%sm", "0");
        }
    }
}

void DisplayBoards(GameState* gamestate) {
	printf("          YOU                 YOUR  OPPONENT\n");
	int ID = -1;
	for (int i = 0 ; i < 10; i++) {
		
		printf("%d",(i + 1));
		if (i < 9) printf("  ");
		else printf(" ");
		
		ID = gamestate->playerID;
		
		for (int j = 0; j < 10; j++) {
			printCell(&gamestate->grids[ID][j][i]);
            printf(" ");
		}
		
		ID = 1 - gamestate->playerID;
        
		printf("  %d",(i + 1));
		if (i < 9) printf("  ");
		else printf(" ");
        
		for (int j = 0; j < 10; j++) {
			printCell(&gamestate->grids[ID][j][i]);
            printf(" ");
		}
		
		printf("\n");
	}
	
	printf("   ");
	for (int k = 0; k < 2; k++ ) {
		for (int i = 0 ; i < 10; i++) {
			printf("%c ", ('A' + i));
		}
		printf("     ");
	}
	printf("\n\n");
    
    switch (gamestate->justTouched) {
        case 0:
            printf("You missed!\n\n");
            break;
        
        case 1:
            printf("You touched!\n\n");
            break;
        
        case 2:
            printf("Your opponent touched!\n\n");
            break;
        
        case 3:
            printf("Your opponent missed!\n\n");
            break;
    }
    
    switch (gamestate->winner) {
        case 0:
            printf("\033[%sm", "35");
            printf("You have lost the game!\n\n");
            printf("\033[%sm", "0");
            break;
        
        case 1:
            printf("\033[%sm", "35");
            printf("Congratulations! You have won the game!\n\n");
            printf("\033[%sm", "0");
            break;
    }
}

void DisplayBoard(GameState* gamestate, int boardID) {
	
	for (int i = 0 ; i < 10; i++) {
		
		printf("%d",(i + 1));
		if (i < 9) printf("  ");
		else printf(" ");

		for (int j = 0; j < 10; j++) {
			printCell(&gamestate->grids[boardID][j][i]);
			printf(" ");
		}
	
		printf("\n");
	}
	
	printf("   ");

		for (int i = 0 ; i < 10; i++) {
			printf("%c ", ('A' + i));
		}

	printf("\n\n");
}