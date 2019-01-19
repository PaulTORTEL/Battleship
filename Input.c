/**
 * Authors: Paul TORTEL & Vincent COURSAC
 * Promo 2019 - Software Engineering
 * 
 * Functions for the input of the keyboard (int, string...)
 *  
 */

#include <stdio.h>  
#include <stdlib.h> 
#include <string.h>

int cleanBuffer(){
    while (getchar()!='\n');
    return 1;
}

int inputRangeInt(int min, int max) {
    int input = 0;
    int error = 0;
    char flush;
    do
    {  
        if (error) printf("Incorrect input, try again.\n");
        printf("\nInput : ");
        error = 1;

    } while (((scanf("%d%c", &input, &flush)!=2 || flush!='\n') && cleanBuffer()) || input<min || input>max);

    return input;
}

char* inputString() {
	fflush(stdin);
	printf("Input : ");
	static char str[BUFSIZ];
	char *temp;

	if (fgets(str, sizeof(str), stdin) != NULL)
	{
		if ((temp = strchr(str, '\n')) != NULL)
		  *temp= '\0';
	}
	return str;
}

int inputCoordinate(int *x, int *y) {
	char* coordinate = inputString();
	
	if (strlen(coordinate) < 2 || strlen(coordinate) > 3 || coordinate[0] < 'A' || coordinate[0] > 'J') {
		return 0;
	}

	*x = (int) coordinate[0] - 'A';
	
	if (*x < 0 || *x > 9) {
        return 0;
	}
		
	if (strlen(coordinate) == 3) {
		if (coordinate[1] == '1' && coordinate[2] == '0') {
			*y = 9;
		}
		else {
			return 0;
		}
	}
	else {
		*y = (int) (coordinate[1] - '1' );
		if (*y < 0 || *y > 8) {
			return 0;
		}
	}
	return 1;
}