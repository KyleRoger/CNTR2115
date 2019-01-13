/*
*  FILE          :	client.h
*  SYSTEM PROJECT:	ispeed
*  SUB-SYSTEM	 :	client
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
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>




/********PROTOTYPES*******/



/********CONSTANTS*******/
#define SUCCESS 1
#define FAILURE 0
#define DEFAULT_PORT 1919