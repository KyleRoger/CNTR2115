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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#ifdef _WIN32
#pragma warning(disable:4996)
#endif


/********PROTOTYPES*******/
int runClient(int argc, char* argv[]);


/********CONSTANTS*******/
#define SUCCESS 1
#define FAILURE 0
