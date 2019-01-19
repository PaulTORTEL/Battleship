#ifndef INPUT_FILE
#define INPUT_FILE

/**
 * Authors: Paul TORTEL & Vincent COURSAC
 * Promo 2019 - Software Engineering
 * 
 * Functions for the input of the keyboard (int, string...)
 *  
 */
 
//Input an int in the specified range (inclusive)
int inputRangeInt(int min, int max);

char* inputString();

int inputCoordinate(int *x, int *y);

//Cleans the in buffer (stdin) in order to secure inputs
int cleanBuffer();

#endif