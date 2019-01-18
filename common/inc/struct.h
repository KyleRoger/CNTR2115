/*
*  FILE          :	struct.h
*  SYSTEM PROJECT:	ispeed
*  SUB-SYSTEM	 :	N/A
*  PROGRAMMER    :	Arie Kraayenbrink
*  Editors	 	 :	Arie Kraayenbrink
*  DATE			 :	January 2019
*  DESCRIPTION   :	This file contains the struct that will be used to store information about the benchmark socket connection
*/



/*******************************CONSTANTS*********************************************/



/******************************STRUCT*************************************/

/*This struct will be used to store information about the benchmark socket connection*/
typedef struct socketStruct				
{
	int socketType;
	int userPort;
	int blockSize;
	int numBlocks;
}socketInfo;
