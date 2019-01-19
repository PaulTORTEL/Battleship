/**
 * Authors: Paul TORTEL & Vincent COURSAC
 * Promo 2019 - Software Engineering
 * 
 * Entry point of the program
 * Fork main process to start the server and the local client
 * or
 * start the remote client to connect to a remote server
 * 
 */

#include <stdio.h>
#include <semaphore.h>
#include <sys/mman.h>
#include "Input.h"
#include "ServerBuilder.h"
#include "ClientBuilder.h"
#include "unistd.h"
#include "utils.h"

int main(int argc, char **argv)
{
	
	printf("============ Battleship ============\n");
    printf("System programming project by\n");
    printf("\tTORTEL Paul, COURSAC Vincent");
    printf("\n====================================\n\n");
    
    printf("1. Host game\n");
    printf("2. Join game\n");
    printf("3. Quit\n");
	
        
    int choice = inputRangeInt(1,3);

	ServerInfo infos;
    
	if (choice == 1) {
        
        // Shared semaphore for processes
        sem_t* sem = (sem_t *)mmap(NULL, sizeof(sem_t*), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1,0);
        sem_init(sem, 1, 0);
        
		infos = prepareServer();
		pid_t serverID = fork();
		
		if (serverID == 0) {
			launchServer(&infos, sem);
			
			// Unset the shared semaphore
            munmap(sem, sizeof(sem_t*));
			
            sem_post(sem);
            
		}
		
		else {
            manageClient(&infos, sem);
            sem_wait(sem);
		}
		
	}
	else if (choice == 2) {
		infos = prepareClient();
        manageClient(&infos, NULL);
	}
	else {
		return 0;
	}
        
    return 0;
}
