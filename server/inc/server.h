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


/******************************STRUCT*************************************/
typedef struct dataStruct
{
	int server_socket;				//the server socket ID
	int clientCount;				//number of clients
	int client_socket;				//the client socket ID
	msgStruct clientMessage;		//the struct containing the clients details and message. - in struct.h
}dataStruct;



/********PROTOTYPES*******/
int getIP(void);
int runServer();
int __cdecl windowsSockets(void);
int newSocket(int* server_socket);
int createSocket(int server_socket);
int closeSocket(int server_socket);
int readClient(dataStruct *infoStruct);
void *socketThread(void *clientSocket);



/********CONSTANTS*******/
#define SUCCESS 1
#define FAILURE 0
