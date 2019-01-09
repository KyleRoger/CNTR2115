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
#include "../../common/inc/ispeed.h"



/********PROTOTYPES*******/
/********CONSTANTS*******/
#define SUCCESS 1
#define FAILURE 0
