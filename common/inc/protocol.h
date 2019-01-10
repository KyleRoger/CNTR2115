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
#define PORT 			15000											//The port to use in communication, chosen at random.
#define BUFFER_SIZE		1024											//The size of the message array that is sent in the socket
#define IP_SIZE			20												//Number of elements in the message array reserved for the IP address
#define IP_START		0												//The starting element in the message array for the IP address
#define NAME_SIZE		80												//Number of elements in the message array reserved for the name
#define NAME_START		(IP_SIZE + 1)									//The starting element in the message array for the name
#define MESSAGE_SIZE	100												//Number of elements in the message array reserved for the actual message
#define MESSAGE_START	(IP_SIZE + NAME_SIZE + 1)						//The starting element in the message array for the actual message
#define TIME_SIZE		20												//Number of elements in the message array reserved for the time
#define TIME_START		(IP_SIZE + NAME_SIZE + MESSAGE_SIZE + 1)		//The starting element in the message array for the time
#define DEFAULT_BUFLEN 	512