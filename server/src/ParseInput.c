
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
#include "../inc/encodeInput.h"



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
int parseCmdLine(int argc, char** argv, char* inputFileName, char* outputFileName)
{
	//set up flags with initial values.
	flags.oFlag = NOT_USED; //indicates if the o flag has been used or not. 
	flags.iFlag = NOT_USED; //indicates if the i flag has been used or not.
	flags.sFlag = NOT_USED; //indicates if the srec flag has benn used or not.
	flags.hFlag = NOT_USED;	//indicates if the h flag has been used or not.

	int retCode = 0; //return code to let calling function know what to do.
	int i = 0;
	int filenameLocation = 0;
	
	//check if help was requested
	for (int i = 0; i < argc; i++)
	{
		if (argv[i][0] == '-' && argv[i][1] == 'h' && argv[i][2] == '\0') //look for a -h switch
		{
			#ifdef DISPLAY_MSG
			printf("Help message requested\n");
			printf("currently, i is %i and argc is %i\n", i, argc);
			#endif

			flags.hFlag = USED;
		
			//finished checking
			retCode = HELP_MSG;
			i = argc + 1; 
		}	
		
		#ifdef DISPLAY_MSG
		printf("Inside the a for loop, checking for help flag, retCode is now %i\n", retCode);
		printf("currently, i is %i and argc is %i\n", i, argc);
		#endif
	}
	
		#ifdef DISPLAY_MSG
		printf("Done cheching for the help flag, retCode is now %i\n", retCode);
		printf("currently, i is %i and argc is %i\n", i, argc);
		#endif
	
	//check that only switches were provided as arguments
	for (int i = 1; i < argc; i++)
	{
		if (argv[i][0] != '-') //look for anything that doesn't start with a dash
		{
			#ifdef DISPLAY_MSG
			printf("Found an invalid agrument. It is %s\n", argv[i]);
			printf("currently, i is %i and argc is %i\n", i, argc);
			#endif
			
			flags.hFlag = ON;

			retCode = INVALID_INPUT;
		}
		
		#ifdef DISPLAY_MSG
		printf("Inside a for loop, checking for invalid arguments, retCode is now %i\n", retCode);
		printf("currently, i is %i and argc is %i\n", i, argc);
		#endif
	}
	
	#ifdef DISPLAY_MSG
	printf("Done check that all arguments contained switches, retCode is now %i\n", retCode);
	printf("currently, i is %i and argc is %i\n", i, argc);
	#endif
	
	//check for valid number of command line arguments
	if (retCode == 0) //check if we should do anymore parsing. Did earlier checking find an error?
	{
		//check if arguments were provided
		if (argc < 2) //no arguments means we will be using stdin and stdout
		{
			strcpy(inputFileName, "stdin");
			strcpy(outputFileName, "stdout");
			
			retCode = ENCODE;
		
			#ifdef DISPLAY_MSG
			printf("No command line arguments given by user.\n"); 
			#endif
		}
	
		//check if too meny arguments were provided
		if (argc > MAX_EXPECTED_ARGUMENTS) //too many arguments
		{
			retCode = INVALID_INPUT;

			flags.hFlag = ON;
		
			#ifdef DISPLAY_MSG
			printf("Error: Too many command line arguments.\n");
			#endif
		}
	}
	
	//check what arguments have been provided
	
	/*****should add ability to check if the same flag has been used over again more then once*****/
	
	if (retCode == 0) //check if we should do anymore parsing. Did earlier checking find an error.
	{		
		//first look for the i flag
		for (int i = 0; i < argc; i++)
		{
			if (argv[i][0] == '-' && argv[i][1] == 'i') //i flag on
			{		
				strcpy(inputFileName, (argv[i] + 2)); //save the input filename and strip the flag from the beginning
				
				#ifdef DISPLAY_MSG
				printf("argv is %s, input file name is %s\n", argv[i], inputFileName);
				#endif
				
				#ifdef DISPLAY_MSG
				printf("argv is %s, input file name is %s\n", argv[i], inputFileName);
				#endif
				
				flags.iFlag = USED;
				
				retCode = retCode + I_FLAG_SET;
				i = argc; //quit loop
				break;
			}
			else
			{
				strcpy(inputFileName, "stdin"); //input file not specified, so use stdin
			}
		}
		
		//now check for the o flag
		for (int i = 0; i < argc; i++)
		{	
			if (argv[i][0] == '-' && argv[i][1] == 'o') //o flag on
			{
				strcpy(outputFileName, (argv[i] + 2)); //save the output filename and strip the flag from the beginning
				
				flags.oFlag = USED;
				
				retCode = retCode + O_FLAG_SET;

				i = argc; 
				break;	//quit loop
				
			}
			else if (flags.iFlag == USED && flags.oFlag == NOT_USED)
			{
				strcpy(outputFileName, inputFileName); //name output file after input file
			}
			else
			{
				strcpy(outputFileName, "stdout"); //no filename provided at all so use stdout
			}
		}
				
		//finally check for the srec flag
		for (int i = 0; i < argc; i++)
		{
			//if the srec is set, and a filename has been provided, append srec to the filename
			if (strcmp(argv[i], "-srec") == 0) //srec flag on
			{
				if (flags.oFlag == USED) //output file name was proveded
				{
					/*if (strstr(outputFileName, ".srec") == NULL) //make sure that .srec isn't already on the file name.
					{
						strcat(outputFileName, ".srec"); //using input file name with .srec extention
					}*/
					flags.sFlag = USED;
					retCode = retCode + SREC_FLAG_SET;
					i = argc; //quit loop
					break;
				}
				else if (flags.iFlag == USED) //output not specified, but the input was
				{ 
					if (strstr(outputFileName, ".srec") == NULL) //make sure that .srec isn't already on the file name.
					{
						strcat(outputFileName, ".srec"); //using input file name with .srec extention
					}
				}
				
				flags.sFlag = USED;
				retCode = retCode + SREC_FLAG_SET;
				i = argc; //quit loop
			}
		}
			
		if (flags.sFlag == NOT_USED) //if a filename has been provided, but srec is not set, append asm to the filename.
		{
			if (flags.oFlag == USED) 
			{
				/*if (strstr(outputFileName, ".asm") == NULL) //make sure that .asm isn't already on the file name.
				{
					strcat(outputFileName, ".asm");
				}*/
			}
			else if (flags.iFlag == USED) //name after the input file name with the .asm
			{
				if (strstr(outputFileName, ".asm") == NULL) //make sure that .asm isn't already on the file name.
				{
					strcat(outputFileName, ".asm");
				}
			}
		}
		
		#ifdef DISPLAY_MSG
		printf("iFlag is %i, oFlag is %i, sFlag is %i\n", flags.iFlag, flags.oFlag, flags.sFlag);
		#endif
		
		if (retCode > ALL_FLAGS_SET) //There was a problem with the command line arguments
		{
			#ifdef DISPLAY_MSG
			printf("Command line flags added up to %i\n", retCode);
			#endif
			
			retCode = ALL_FLAGS_SET;
		}
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
void showHelp(char** argv, int whichHelp)
{
	switch(whichHelp)
	{
	
	case HELP_MSG: //This is basically just a quck usage statement.
		//show PROGRAM useage
		printf("Usage:\t%s <arguments>\n", argv[0]);
		printf("\t\t\t-i<filename>\t: the input file name\n");
		printf("\t\t\t-o<filename>\t: the output file name\n");
		printf("\t\t\t-srec\t\t: use .srec extention on output file\n");
		printf("\t\t\t-h\t\t: this help message\n \n");
		break;
		
	default:
		break;
	}
} //end showHelp function
		
		

/* 	
*	Function Name	: runUtility(int argc, char** argv)
*	Description		: This function controlls what is done in the program. 
*					: It uses other functions to parse the command line and then calls the aproprate function to do the work.
*					: The return code from the parse function is the main indicator of what function should be called next.
*					:
* 	Parameters		: int argc - number of command line arguments
*					: char** argv - where the command line arguments are
*					: 
*	Returns			: int retCode - type of input, valid or otherwise, provided as arguments in the command line.
*/
int runUtility(int argc, char** argv)
{
	int inputVal = 0; 							//the return status from parse function
	char inputFileName[kNameLength] = { 0 }; 	//the file name to use for input data
	char outputFileName[kNameLength] = { 0 }; 	// the file name to use for output data
	int errorCode = 0; 							// the function return value.
	
	//parse the command arguments provided in the command line by the user.
	inputVal = parseCmdLine(argc, argv, inputFileName, outputFileName);
	
	#ifdef DISPLAY_MSG
	printf("Return code from parseCmdLind is %i\n", inputVal);
	printf("input file name has been set to %s\n", inputFileName);
	printf("output file name has been set to %s\n", outputFileName);
	#endif 
	
	if (flags.hFlag == USED)
	{
		showHelp(argv, HELP_MSG); //print help message
		errorCode = inputVal;
		return errorCode;
	}

/*	if (flags.iFlag == NOT_USED && flags.oFlag == NOT_USED)
	{
		errorCode = encodeASM("stdin", "stdout"); //use stdin and stdout rather then files
		return errorCode;
	}

	if (flags.iFlag == USED)
	{
		if (flags.sFlag == USED) //check if srec or asm file output is desired
		{
			errorCode = encodeSREC(inputFileName, outputFileName); //output to .srec file
		}
		else
		{
			errorCode = encodeASM(inputFileName, outputFileName); //output to .asm file
		}

		return errorCode;
	}

	if (flags.oFlag == USED)
	{
		if (flags.sFlag == USED)
		{
			errorCode = encodeSREC(inputFileName, outputFileName); //use srec encoding scheme
		}
		else
		{
			errorCode = encodeASM(inputFileName, outputFileName); //use asm encoding scheme
		}

		return errorCode;
	}*/




	if (inputVal == INVALID_INPUT) //something other then a switch was used on the command line.
	{
		showHelp(argv, HELP_MSG); //print help message
		errorCode = INVALID_INPUT;
		return errorCode;
	} 
	else if (inputVal == HELP_MSG) // h flag has been used
	{
		showHelp(argv, HELP_MSG); //print help message
		errorCode = HELP_MSG;
		return errorCode;
	}
	else if (inputVal == I_FLAG_SET) //only the i flag has been used
	{
		if (flags.sFlag == USED) //check if srec or asm file output is desired
		{
			errorCode = encodeSREC(inputFileName, outputFileName); //output to .srec file
		}
		else
		{
			errorCode = encodeASM(inputFileName, outputFileName); //output to .asm file
		}
	}
	else if (flags.oFlag == USED) // only the o flag has been used
	{
		if (flags.sFlag == USED)
		{
			errorCode = encodeSREC(inputFileName, outputFileName); //use srec encoding scheme
		}
		else
		{
			errorCode = encodeASM(inputFileName, outputFileName); //use asm encoding scheme
		}
	}
	else if (inputVal == I_FLAG_SET + O_FLAG_SET) // i and o flags used
	{
		if (flags.sFlag == USED)
		{
			errorCode = encodeSREC(inputFileName, outputFileName); //use srec encoding scheme
		}
		else
		{
			errorCode = encodeASM(inputFileName, outputFileName); //use asm encoding scheme
		}
	}
	else if (inputVal == SREC_FLAG_SET) // Only the srec flag used
	{
		errorCode = encodeSREC("stdin", "stdout");
	}
	else if (inputVal == ALL_FLAGS_SET) //all flags except the h flag are on
	{
		errorCode = encodeSREC(inputFileName, outputFileName); //use srec encoding scheme
	}
	else if (inputVal > ALL_FLAGS_SET) //all flags are on
	{
		//errorCode = encodeSREC(inputFileName, outputFileName); //use srec encoding scheme
		showHelp(argv, HELP_MSG); //print help message
		errorCode = INVALID_INPUT;
		return errorCode;
	} 
	else if (inputVal == ENCODE) //no command line switches provided
	{
		errorCode = encodeASM("stdin", "stdout"); //use stdin and stdout rather then files
	}
	else if (flags.iFlag == USED)
	{
		if (flags.sFlag == USED)
		{
			errorCode = encodeSREC(inputFileName, outputFileName); //use srec encoding scheme
		}
		else
		{
			errorCode = encodeASM(inputFileName, outputFileName); //use asm encoding scheme
		}
	} 
	else
	{
		showHelp(argv, HELP_MSG); //print help and quit
		errorCode = HELP_MSG;
	}
	
	return errorCode;
} //end runUtility function

