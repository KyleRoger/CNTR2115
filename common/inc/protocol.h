/*
*  FILE          :	protocol.h
*  SYSTEM PROJECT:	ispeed
*  SUB-SYSTEM	 :	
*  PROGRAMMER    :	Arie Kraayenbrink
*  Editors	 	 :	Arie Kraayenbrink
*  FIRST VERSION :	April 13, 2018
*  DESCRIPTION   :	This file contains the constants for the message transfer protocol for the system.
*				 :	The most important item is the port, if there is any conflict of interest, feel free to change it
*				 : 	to a free port. Of coarse, all clients and the server must and do share the same port.
*/



/*******************************CONSTANTS*********************************************/
#define DEFAULT_PORT 		15000			//The default port to use in communication, chosen at random.
#define DEFAULT_SOCK_TYPE	SOCK_STREAM		//Use TCP as the default					
#define BUFFER_SIZE			1024			//The size of the message array that is sent in the socket
#define DEFAULT_BUFLEN 		512
