/*
*  FILE          :	server.h
*  SYSTEM PROJECT:	ispeed
*  SUB-SYSTEM	 :	server
*  PROGRAMMER    :	Arie Kraayenbrink
*  Editors	 	 :	Arie Kraayenbrink
*  FIRST VERSION :	Jan. 8, 2019
*  DESCRIPTION   :	This file contains the files, constants function prototype and even ...
*/



/********PRE-PROCESSORS*******/
#include "../../common/inc/struct.h"
#include "../../common/inc/protocol.h"
#include "../../common/inc/ispeed.h"
#include "../../common/inc/ispeedSockets.h"

#ifdef linux
	#include <ifaddrs.h>
	#include <linux/if_link.h>
#endif



/********PROTOTYPES*******/
int getIP(void);
int runServer();
int __cdecl windowsSockets(void);
int newSocket(int* server_socket);
int createSocket(void);
int closeSocket(int server_socket);



/********CONSTANTS*******/
#define SUCCESS 1
#define FAILURE 0



/******************************STRUCT*************************************/
typedef struct dataStruct
{
	int server_socket;								//the server socket ID
	int client_socket[MAX_CLIENTS + 1];				//the client socket ID
	msgStruct clientMessage[MAX_CLIENTS + 1];		//the struct containing the clients details and message. - in struct.h
	pthread_t 	clientThreadID[MAX_CLIENTS + 1];	//the client thread ID
	pthread_t 	serverThreadID[MAX_CLIENTS + 1];	//the server thread ID
}dataStruct;
