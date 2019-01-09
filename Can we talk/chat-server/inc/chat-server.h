/*
*  FILE          :	chat-server.h
*  SYSTEM PROJECT:	CHAT-SYSTEM
*  SUB-SYSTEM	 :	chat-server
*  PROGRAMMER    :	Arie Kraayenbrink
*  Editors	 	 :	Arie Kraayenbrink
*  FIRST VERSION :	March 31, 2018
*  DESCRIPTION   :	This file contains the files, constants function prototype and even a struct to be used in the CHAT-SYSTEM project.
*				 : 	The struct is used to keep track of our clients and all the detials about them. And just like google, it knows everything
*				 :	about the clients personal life. This includes the clients ID, and the complete message struct as well as the client's threadID.
*				 :	This project is built for Linux O/S and doesn't include the needed setup for other O/Ss such as windows.
*/



/********PRE-PROCESSORS*******/
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>
#include <sys/wait.h>
#include <malloc.h>
#include <stdbool.h>
#include <errno.h>

#include "../../common/inc/struct.h"
#include "../../common/inc/protocol.h"



/******************************STRUCT*************************************/
typedef struct dataStruct
{
	int server_socket;								//the server socket ID
	int clientCount;								//number of clients
	int clientID[MAX_CLIENTS + 1];					//the clients personal ID
	int client_socket[MAX_CLIENTS + 1];				//the client socket ID
	msgStruct clientMessage[MAX_CLIENTS + 1];		//the struct containing the clients details and message. - in struct.h
	pthread_t 	clientThreadID[MAX_CLIENTS + 1];	//the client thread ID
	pthread_t 	serverThreadID[MAX_CLIENTS + 1];	//the server thread ID
}dataStruct;



/*******************************************PROTOTYPES******************************************************/
int runServer(void);
void *socketThread(void *clientSocket);
int monitorClients(dataStruct *infoStruct);
int newSocket(int* server_socket);
int broadcastMSG(dataStruct* clientInfo, int clientID);
void handle_alarm(int clientThreadID);
int removeClient(dataStruct* clientInfo, int clientID);