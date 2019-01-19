#ifndef CLIENT_FILE
#define CLIENT_FILE

#include "game.h"


/**
 * Authors: Paul TORTEL & Vincent COURSAC
 * Promo 2019 - Software Engineering
 * 
 * Game loop function, function to handle the message received,
 * handle play turn (selection of where the player wants to attack)
 * handle ship creation on client side
 * 
 */
 
void launchGame(GameState* gamestate);
void handleMessage(GameState* gamestate, char* raw);
void handleShipCreation(GameState* gamestate);
void handlePlayTurn(GameState* gamestate);
#endif