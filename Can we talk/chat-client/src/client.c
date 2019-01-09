/*
*  FILE          :	client.c
*  SYSTEM PROJECT:	CAN-WE-TALK
*  SUB-SYSTEM	 :	chat-client
*  PROGRAMMER    :	Kyle,Horsley
*  Editors	 	 :	Kyle Horsley, Arie Kraayenbrink
*  FIRST VERSION :	April 18, 2018
*  DESCRIPTION   :	This file contains the main function for the chat-client.
*				 :	main calls the runClient function and waits for it to complete.
*/



#include "../inc/client.h"



int main (int argc, char *argv[])
{
  run_client(argc, argv); //This function will run the client

  return 0;
}