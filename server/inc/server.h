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
#include <time.h>
#include <malloc.h>

#ifdef linux
	#include <ifaddrs.h>
	#include <linux/if_link.h>
#endif



/********CONSTANTS*******/
#define SUCCESS 1
#define FAILURE 0
#define INVALID_PARMS -1
#define RUN 	1
#define EXIT 	0
#define BUFLEN 10000	//Max length of buffer
#define PORT 15008		//The port on which to listen for incoming data

#define DEBUG			//debugging messages



/********PROTOTYPES*******/
void getIP(void);
int runServer(int argc, char *argv[]);
int newSocket(int* server_socket, int sockType, int sockPort);
int intSocket(void);
int readClient(int benchMarkSocket, int numBlocks, int blockSize, int sockType);
void showHelp(char** argv);
int parseCmdLine(int argc, char** argv);
void testType(socketInfo *testConnection, int serverSocket);
int connectonDetails(socketInfo *benchMarkConnection);
int readTCP(socketInfo benchMarkConnection);
int readUDP(socketInfo benchMarkConnection);
