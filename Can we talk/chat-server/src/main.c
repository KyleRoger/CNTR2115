/*
*  FILE          :	main.c
*  SYSTEM PROJECT:	CHAT-SYSTEM
*  SUB-SYSTEM	 :	chat-server
*  PROGRAMMER    :	Arie Kraayenbrink
*  Editors	 	 :	Arie Kraayenbrink
*  FIRST VERSION :	March 31, 2018
*  DESCRIPTION   :	This file contains the main function for the chat-server.
*				 :	main calls the runServer function and waits for it to complete.
*				 :	The return status from runServer is returned to the calling enviroment.
*/



#include "../inc/chat-server.h"



int main(void)
{
	return runServer();
}