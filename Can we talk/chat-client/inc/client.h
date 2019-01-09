/*
*  FILE          :	client.h
*  SYSTEM PROJECT:	CHAT-SYSTEM
*  SUB-SYSTEM	 :	chat-client
*  PROGRAMMER    :	Kyle Horsely
*  Editors	 	 :	Kyle Horsley, Arie Kraayenbrink
*  FIRST VERSION :	April 18, 2018
*  DESCRIPTION   :	This file contains the files, constants function prototype and even a struct to be used in the CHAT-SYSTEM project.
*				 : 	The struct is used to keep track of our clients and all the detials about them. This includes the clients ID, 
*				 :	and the complete message struct as well as the client's threadID.
*				 :	The struct also contains information for using ncurses to draw the screen in the desired format.
*				 :	This project is built for Linux O/S and doesn't include the needed setup for other O/Ss such as windows.
*/



/********PRE-PROCESSORS*******/
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdarg.h>
#include <fcntl.h>
#include <ncurses.h>
#include <pthread.h>
#include <stdbool.h>
#include <ifaddrs.h>
#include <sys/time.h>

#include "../../common/inc/struct.h"
#include "../../common/inc/protocol.h"



/*******************************CONSTANTS*********************************************/
#define CONTINUE		true 				//Determines whether to continue threaing or not
#define EXIT			1					//Returns whether to exit the program	
#define SPLIT_POINT		40					//The point at which to split the string
#define BYE           	">>bye<<"			//The exit phrase
#define CHAT_HEADER		"CHAT LOG"			//The header to go above the chat log window.
#define MESSAGE_HEADER	"ENTER MESSAGE:"	//The header to go above the messge window.
#define IP_POS        	1					//The position that the IP will be displayed when printing to the screen.
#define NAME_POS      	16					//The position that the name will be displayed when printing to the screen.
#define DIRECTION_POS 	24					//The position the directional arrows will be displayed when printing to the screen.
#define MSG_POS			28					//The position the message will start at when being printed.
#define TIME_POS      	69					//The position the time will be displayed when printing to the screen.

#define _REENTRANT							//Used to re-enter the thread if using a function within


char buffer[BUFSIZ];						//Global variable that will store the input up to 80 characters


/******************************STRUCT*************************************/

/*This struct will hold the data that will be used wihtin the threading function
This data will be used to determine which client sent the information and which
socket to read from*/
typedef struct dataStruct
{
	char 		myIP[IP_LENGTH +1];			//The IP of the client running the application
  	char    	myName[NAME_LENGTH + 1];    //The sname of the client running the application
  	char 		messageSendOrRecieve[3];	//The direction of the arrows ">>" "<<"
	msgStruct 	clientInformation;			//The struct that contains the sending client's information - in struct.h
	int 		server_socket;				//The socket connected too.
	WINDOW* 	chat_win;					//The ncurses window that the chat will be displayed in.
}dataStruct;

/*******************************************PROTOTYPES******************************************************/
void* my_thread_reading_function (void *my_server_socket);
int run_client(int argc, char* argv[]);
WINDOW *create_newwin(int height, int width, int starty, int startx, char* screenMessage);
void splitStr(char message[(MSG_SIZE + 1)]);
void getIP( char **addr);
int getSocket(struct hostent* host);
void printSocketInfo(struct dataStruct *information, int curRow);
void clearChar(char buf[]);