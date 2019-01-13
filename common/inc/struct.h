/*
*  FILE          :	struct.h
*  SYSTEM PROJECT:	CHAT-SYSTEM
*  SUB-SYSTEM	 :	N/A
*  PROGRAMMER    :	Arie Kraayenbrink
*  Editors	 	 :	Arie Kraayenbrink
*  FIRST VERSION :	April 5, 2018
*  DESCRIPTION   :	This file contains the message struct for our system. The struct holds all the information and is the way the info is shared.
*				 :	Within the struct, we have the sending client's IP, the sending clients name and the actual message from that client.
*				 :	The struct is passed from the client to the server, the server then broadcasts the message to all attached clients.
*				 :	Once the client receives any message it prints it to the screen.
*				 :
*				 :	Note: Because this system is designed to run on matching O/S and architecture, passing a struct is safe. However, using this
*				 :	on other types of system architectures is not recommended and may cause undesirable results.
*/



/*******************************CONSTANTS*********************************************/
#define MAX_CLIENTS		10		//The max number of clients that are supported at a time
#define NAME_LENGTH		5		//The max number of chars that a name can have.
#define MSG_SIZE		80		//The max number of chars that the message may have.
#define IP_LENGTH		16		//The length of the IP address



/******************************STRUCT*************************************/

/*This struct will be used to send messages to the server. It includes the
sending client's IP address, name and the message and will be broadcasted 
to all users*/
typedef struct messageStruct				
{
	char 		IP[IP_LENGTH + 1];			//the sending client's IP address
	char 		name[NAME_LENGTH + 1];		//The sending client's name
	char 		message[MSG_SIZE + 1];		//The message that the client sent
}msgStruct;
