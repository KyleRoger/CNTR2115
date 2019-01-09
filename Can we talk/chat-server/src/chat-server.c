/*
*  FILE          :	chat-server.c
*  SYSTEM PROJECT:	CHAT-SYSTEM
*  SUB-SYSTEM	 :	chat-server
*  PROGRAMMER    :	Arie Kraayenbrink
*  Editors	 	 :	Arie Kraayenbrink
*  FIRST VERSION :	March 31, 2018
*  DESCRIPTION   :	This file contains the functions and code to run the chat server. One server will support up to a max of 10
*				 :	clients at once. 
*				 :
*				 :	The Chat system is designed for the Linux operating system and allows users to chat with one another as long as they are on the same network.
*				 :	Note: Because this system is designed to run on matching O/S and architecture, passing a struct is safe. However, using this
*				 :	on other types of system architectures is not recommended and may cause undesirable results.
*				 :
*				 :	The basic operation of the system is a follows. A user starts a client and enters a user name and the server IP address. The user can then enter a
*				 :	message to send to all other clients that are connected to the system. To exit their client, the user inputs >>bye<<
*				 :
*				 :	The server silently tracks each connected client and whenever it receives a message from a client it will broadcast that message to all connected clients.
*				 :	The protocol used in the message transfer provides all the client information within a struct so that the server knows who is who. The server doesn't do
*				 : 	much with the message other then read and then broadcast it out to the other clients it has on file. This client information including its thread ID is 
*				 :	stored in the datastruct owned by the server. More details on the data struct can be found in chat-server.h
*				 :
*				 :	Once all clients have left the server, it simply shuts down. Note, a server must be running at all times for the chat system to work. Without the server
*				 :	clients won't be able to attach to a socket that isn't in existence and messages can not be able to be sent.
*				 :	The server supports a max of 10 clients at any given time. It is a multi threaded application, the main thread monitors for new clients, well each 
*				 : 	client is given its own thread that reads messages and broadcasts those messages to other clients.
*				 :
*				 :	The client is started with two command line arguments, a name and the servers IP address.
*				 :	The client has two windows, one for messages received and one for entering messages to send. It runs with two threads, one for each of the windows. 
*				 :	Each message is displayed with the senders IP and name, as well, all messages are timestamped when they are revived.
*				 :	Once a client has attached to the server and receives a message, the client goes to work at formating the message as needed and displays it to the screen. 
*				 :	The output is formated for the screen so that there is a max of 40 characters per line.
*				 :	Where possible, the line is split at the nearest space that is less then 40 characters. Ideally,the split of a line over 40 characters will happen at at 
*				 :	the nearest space less then 40 characters. However, if splitting the message at this point would cause the second line to exceed the 40 character limit,
*				 :	the line will be split at 40 characters. This splitting takes place in the client when a message is received. The client is also responsible for putting a 
*				 :	time stamp on all received messages when displaying them to the screen.
*				 :
*				 : 	This system uses a custom protocol based on sockets to transfer the chat messages. The socket uses a custom port number specified in the protocol.h header file. 
*				 :	Each message is contained within a struct that also holds the client's IP and name. 
*				 :	The IP is a IPv4 type address of the clients machine. It is a four octet address such as 192.168.1.131. 
*				 :	The name is what was entered as the first argument when starting the client. It is a maximum of 5 characters in length. 
*				 :	The message is a string of text entered by the user. It is a maximum of 80 characters and will be displayed with a max of 40 characters per line. 
*				 :	No other information is transmitted about the client in this struct.
*				 :	
*				 :	All of this information is voluntarily provided by the client who packages it up and sends it to the server. The server logs the clients information into
*				 :	its datastruct. As stated previously, the server then creates a thread for the client and then sends out any messages from that client to all other clients attached. 
*				 :	The moment a message is received, the server checks if the message is ">>bye<<", if it is the socket is closed and the information in the server's struct is cleaned up.
*				 :	However, it the message is not ">>bye<<", the server does a loop that sends the message to all clients on record. Once all clients have left the last thread sends a 
*				 :	SIGALRM the exits the thread and closes the program.
*				 :	
*				 :	See the respective header files for more specifics on the struct and the protocol used. 
*/



//preprocessors
#include "../inc/chat-server.h"

static int numClients = 0;	//used to track the number of clients attached.



/* 	
*	Function Name	: runServer(void)
*					:
*	Description		: This function does most of the coordinating and calls the shots.
*					: The basic flow of things is, create and bind to a socket, listen on that socket,
*					: create a thread for each new client and lastly join threads, close sockets and leave.
*					: 
* 	Parameters		: N/A
*					:
*	Returns			: int retCode: the return value indicating the success or failure of the function
*/
int runServer(void)
{
	int retCode 			= 0; 		//the return value indicating the success or failure of the function
	dataStruct clientInfo	= { 0 };	//This struct is used to hold messages sent from clients.
	int 	server_socket 	= 0;		//This is the socket used by the server

	//set up a new socket and bind to it
	newSocket(&server_socket);

	clientInfo.server_socket = server_socket;

	/*
   	* start listening on the socket
   	*/
	if (listen (server_socket, 5) < 0) 
	{
		printf ("[SERVER] : listen() - FAILED.\n");
		close (server_socket);
		return 3;
	}
	
	//check for clients
	monitorClients(&clientInfo);

   	// let's go into a busy "join" loop waiting for
	// all of the clients to finish and join back up to this main thread
	for(int i = 0; i < numClients; i++)
	{
	    if(pthread_join(clientInfo.clientThreadID[i], (void *)(&retCode)))
	    {
	       printf("[SERVER] : pthread_join() FAILED\n");
	       return 6;
	    }
	}

	close (server_socket);

	return 	retCode;
} //end runServer function



/* 	
*	Function Name	: void *socketThread(void *clientSocket)
*					:
*	Description		: This function is a separate thread that is created for each client. It checks for incoming messages
*					: from the client and then calls a function to broadcast that message to all other clients.
*					: 
* 	Parameters		: void *clientSocket : A void pointer to the struct that has all the client and server information
*					: as well as the struct used in our communication protocol.
*					:
*	Returns			: int retCode: the return value indicating the success or failure of the function
*/
void *socketThread(void *clientSocket)
{
  dataStruct* clientInfo = (dataStruct*) clientSocket;
  int cID = clientInfo->clientID[numClients - 1];	//Used for tracking the clients ID
  int clSocket = clientInfo->client_socket[cID - 1];

  clientInfo->clientCount++;	//Track number of clients

  //read in the message from the client
  read (clSocket, &clientInfo->clientMessage[cID - 1], sizeof(clientInfo->clientMessage));

  clientInfo->client_socket[cID - 1] = clSocket;
  int done = 1;

  //loop till client enters >>bye<<
  while((done = strcmp(clientInfo->clientMessage[cID - 1].message,">>bye<<")) != 0)
  {
  	
    if (done != 0)
    {
    	//broadcast struct to all clients
    	broadcastMSG(clientInfo, cID);
    }
    else
    {
    	break;
    }
	
	//read in the message from the client
	read (clSocket, &clientInfo->clientMessage[cID - 1], sizeof(clientInfo->clientMessage));
	
  }
  
  // decrement the number of clients
  numClients--;

  if (numClients == 0)
  {
  	//clean up the struct
	removeClient(clientInfo, cID);

  	//shut server down
  	close(clSocket);
  	close (clientInfo->server_socket);

  	signal( SIGALRM, handle_alarm ); // Install handler first
    kill(getpid(), SIGALRM);
  }
  else
  {
  	//clean up the struct
	removeClient(clientInfo, cID);
	close(clSocket);
  }
	
  return 0;
}



/* 	
*	Function Name	: newSocket(int* server_socket)
*					:
*	Description		: This function sets up and binds to a socket. If theres is any problems doing this, 
*					: an error is printed to the screen.
*					: 
* 	Parameters		: int* server_socket : The socket that is about to be set up. It will be used by the server.
*					:
*	Returns			: int retCode: the return value indicating the success or failure of the function
*/
int newSocket(int* server_socket)
{
	int retCode 			= 0; 		//the return value indicating the success or failure of the function
	struct sockaddr_in server_addr;		//A struct used for the socket information.

	//set up socket
	if ((*server_socket = socket (AF_INET, SOCK_STREAM, 0)) < 0) 
	{
		printf ("[SERVER] : socket() FAILED. \nErrno returned %i\n", errno);
		retCode = -1;
	}
	else
	{
		//bind to socket
		memset (&server_addr, 0, sizeof (server_addr));
		server_addr.sin_family = AF_INET;
		server_addr.sin_addr.s_addr = htonl (INADDR_ANY);
		server_addr.sin_port = htons (PORT);

		if (bind (*server_socket, (struct sockaddr *)&server_addr, sizeof (server_addr)) < 0) 
		{
			printf ("[SERVER] : bind() FAILED. \nErrno returned %i\n", errno);
			close (*server_socket);
			retCode = -2;
		}

	}

	return retCode;
} //end newSocket function



/* 	
*	Function Name	: int monitorClients(void *clientSocket)
*					:
*	Description		: This function looks for new sockets that indicate a client wants to attach to the server.
*					: It loops till the number of clients decrees to zero or it get killed. After picking up a
*					: new client, it creates a thread for that client so the server can handle messages from it.
*					: 
* 	Parameters		: void *infoStruct : This is the struct that has all the data for the client and server.
*					: It also holds another struct that is used for sending messages back and forth.
*					:
*	Returns			: int retCode: the return value indicating the success or failure of the function
*/
int monitorClients(dataStruct *infoStruct)
{
	dataStruct* clientInfo 	= (dataStruct*) infoStruct;		//A pointer to the struct that has all the client and server info.
	int 	retCode			= 0;							//The return value indicating the success or failure of the function.
	int 	client_socket 	= 0;							//The client's socket, set by the accept call.
	int     client_len		= 0;							//The size of the client_addr struct.
	int 	server_socket 	= clientInfo->server_socket;	//The server's socket.
	struct 	sockaddr_in client_addr; 						//Struct with details about client socket.
  	
	//get clients and give them their own thread to work with.
	do 
  	{

  		if (numClients <= MAX_CLIENTS) //do nothing for new clients well we have the max clients attached.
  		{

	       	/*
			* accept a packet from the client.
			* this is a blocking operation.
			*/
			client_len = sizeof (client_addr);
			if ((client_socket = accept (server_socket,(struct sockaddr *)&client_addr, &client_len)) < 0) 
			{
		          printf ("[SERVER] : accept() FAILED\n");
		          fflush(stdout);	
		          break;
			}

			//track clients
			numClients++;
			clientInfo->client_socket[numClients - 1] = client_socket;
			clientInfo->clientID[numClients - 1] = numClients;
		    
	        /*
	        * ... we'll create a thread of execution within this task to take
	        * care of reading and writing to each attached client.
	        * Each and every client gets their own thread... 
	        */
			if ((pthread_create(&(clientInfo->clientThreadID[(numClients-1)]), NULL, socketThread, clientInfo)) != 0) 
			{
			  printf ("[SERVER] : pthread_create() FAILED\n");
		      fflush(stdout);	
		      
		      retCode = -5;
		      break;
			}
		} 
		else
		{
			printf ("Error, reached max number of supported clients.\n");
		}

	}while (numClients >= 1); //loop as long as there are clients attached.

	return retCode;
} //end monitorClients function



/* 	
*	Function Name	: int broadcastMSG(dataStruct* clientInfo, int clientThreadID)
*					:
*	Description		: This function takes in the struct that was read from a client and 
*					: broadcasts it out to all attached clients.
*					: 
* 	Parameters		: dataStruct* clientInfo : The struct that has all the server and client info as well as another struct with the message.
*					: int clientThreadID : The thread ID of the client that is sending the message.
*					:
*	Returns			: int retCode: the return value indicating the success or failure of the function
*/
int broadcastMSG(dataStruct* clientInfo, int clientThreadID)
{
	int retCode = 0; 		//the return value indicating the success or failure of the function

	//loop through all clients, sending message to each client
	for (int i = 0; i < numClients; i++)
    {
    	//send message struct
     	if ((write (clientInfo->client_socket[i], &clientInfo->clientMessage[clientThreadID - 1], sizeof(clientInfo->clientMessage[clientThreadID - 1]))) == -1)
    	{
    		printf("Error writing message to client %i.\n", clientInfo->clientID[i]);
    		retCode = -1;
    	}
    }

	return retCode;
} //end broadcastMSG



/* 	
*	Function Name	: handle_alarm(int clientThreadID) 
*					:
*	Description		: This function joints threads and then closes the thread which ends the program.
*					: This function is only call by a SIGALRM. When all clients have detached from the 
*					: server, a SIGALRM will be generated.
*					: 
* 	Parameters		: int clientThreadID : The threadID of the client that called this function.
*					:
*	Returns			: N/A
*/
void handle_alarm(int clientThreadID) 
{ 
	//all done, good bye 
	kill(getpid(), SIGKILL);

} //end handle_alarm function



/* 	
*	Function Name	: removeClient
*					:
*	Description		: This function removes a client from the struct and collapses the struct so there are no empty gaps.
*					: It does this by moving each client one element down, then it sets the last one to 0.
*					: 
* 	Parameters		: dataStruct* clientInfo	: The struct containing the details of the client that we wish to remove.
*					: int* clientID				: The ID of the client that we want to get rid of. 
*					:
*	Returns			: int retVal: the return value indicating the success or failure of the function 
*/
int removeClient(dataStruct* clientInfo, int clientID)
{
	int retVal 	= 0;							//The success status of this function.
	int cID		= (clientID - 1);				//A local variable used to locally track the client ID.

	for (; cID < clientInfo->clientCount; cID++)
	{
		//move clients down in the list deleting the inactive client.
		clientInfo->clientMessage[cID] 	= clientInfo->clientMessage[cID + 1];
		clientInfo->clientID[cID] 		= clientInfo->clientID[cID + 1];
		clientInfo->client_socket[cID] 	= clientInfo->client_socket[cID + 1];
		clientInfo->clientThreadID[cID] = clientInfo->clientThreadID[cID + 1];
	}
	
	//set last client to zero since it has already been moved down		
	strcpy(clientInfo->clientMessage[cID].IP, "0");
	strcpy(clientInfo->clientMessage[cID].name, "0");
	strcpy(clientInfo->clientMessage[cID].message, "0");

	clientInfo->clientCount--;

	return retVal;	
} //end removeClient function