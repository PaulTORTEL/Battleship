#ifndef GAME_FILE
#define GAME_FILE

#include <semaphore.h>


/**
 * Authors: Paul TORTEL & Vincent COURSAC
 * Promo 2019 - Software Engineering
 * 
 * Functions for the game : game state structure to keep track of the progression of the game
 * update board function called when a position has been shot
 * input ships functions called when players are ready to play
 * not used anymore functions to place by default ships
 */
 
typedef struct {
  int type;
  int touched;
} GridState;

typedef struct {
  GridState grids[2][10][10];
  int playerID;
  int running;
  int state;
  int client;
  sem_t *semaphore;
  int justTouched;
  int winner;
  
} GameState;

GameState* newGameState();

void DisplayBoards(GameState* gamestate);
void DisplayBoard(GameState* gamestate, int boardID);
void updateBoard(GameState* gamestate, int x, int y, int boardID, int touched);

void inputShips(GameState* gamestate);

void fillDefaultShips(GameState* gamestate);

#endif