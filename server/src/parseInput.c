
/*************************************************************************************************************************************
** File			: input.c
** Project		: encodeInput
** Author		: Arie Kraayenbrink (5698998)
** Date			: Feb. 15, 2018
** Description	: This file contains the functions to deal with the command line arguments and decide what kind or record the user
**				: wants. It has several functions that parse the data and call the apropreate function to do the encoding. 
**				: If no input file is named, stdin is assumed. The same for output, if it wasn't provided, use stdout. 
**				: Also there are two types of file outputs - srec or asm. Only one is used at a time and is indicated by a switch.
**************************************************************************************************************************************/



//preprocessor
#include "../inc/server.h"



/* 	
*	Function Name	: parseCmdLine
*	Description		: This function takes in agruments from the command line and seperate them into useful parts.
*					: It also does checks for invalid input. If no input file is named, stdin is assumed. The
*					: same for output, if it wasn't provided, use stdout. Also there are two types of file outputs
*					: - srec or asm. Only one is used at a time.
*					: Then it saves the arguments status for later use before returning the outcome of checks made. 
* 	Parameters		: int argc - number of command line arguments
*					: char** argv - where the command line arguments are
*					: char* inputFileName - name of the file provided for input at the command line.
*					: char* outputFileName - name of the file provided at the command line for output.
*	Returns			: int retCode - type of input, valid or otherwise, provided as arguments in the command line.
*/
int parseCmdLine(int argc, char** argv, int* port)
{

	int retCode = 0; //return code to let calling function know what to do.
	int i = 0;
	
	if (argc != 3)
	{
		showHelp(argv); //print help and quit
		retCode = INVALID_PARMS;
	}
	else if (argv[1][0] == '-' && argv[1][1] == 'p' && argv[1][2] == '\0') //look for a -p switch
	{
		if (!isdigit(argv[2]))
		{
			showHelp(argv); //print help and quit
			retCode = INVALID_PARMS;
		}
		else
		{
			port = atoi (argv[2]);
			retCode = SUCCESS
		}

		//finished checking
		i = argc + 1; 
	}

	return retCode;
} //end parseCmdLine function



/* 	
*	Function Name	: showHelp
*	Description		: Display a help message to the screen.
*					: Currently this function only deals with one help message
*					: This message is basically just a quck usage statement.
*					:
* 	Parameters		: char** argv - what the command line arguments are.
*					: int whichHelp - what message should be printed out to the screen.
*					: 
*	Returns			: N/A
*/
void showHelp(char** argv)
{
	//This is basically just a quck usage statement.
	//show PROGRAM useage
	printf("Usage:\t%s <arguments>\n", argv[0]);
	printf("\t\t\t-p <port>\t: the port number for socket communication\n");

} //end showHelp function
