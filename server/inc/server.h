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



/********PROTOTYPES*******/
int getIP(void);
int runServer();
int windowsSockets(void);



/********CONSTANTS*******/
#define SUCCESS 1
#define FAILURE 0
