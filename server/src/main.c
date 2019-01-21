/*
* -----------------------------------------------------------------------
* File			: main.c
* Project		: ispeed
* Author 		: Arie Kraayenbrink
* Editors		: Arie kraayenbrink
* Date 			: Jan. 8, 2019
* Description 	: This in the main file for ispeed. It holds the main function and calls the runServer function. 
*
* ------------------------------------------------------------------------
*/



#include "../inc/server.h"



int main(int argc, char *argv[])
{
	return runServer(argc, argv);	// runServer is located in server.c
}