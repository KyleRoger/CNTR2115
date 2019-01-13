/*
* -----------------------------------------------------------------------
* File			: makefile
* Project		: ispeed
* Author 		: Arie Kraayenbrink
* Editors		: Arie kraayenbrink
* Date 			: Jan. 8, 2019
* Description 	: This makefile will compile and link all source files
*             	: in the server part of the system.  
*				: 
*				: Steps for creating windows sockets on server.
*				   1. Initialize Winsock.
*				   2. Create a socket.
*				   3. Bind the socket.
*				   4. Listen on the socket for a client.
*				   5. Accept a connection from a client.
*				   6. Receive and send data.
*				   7. Disconnect.
*
* Credit		: https://docs.microsoft.com/en-us/windows/desktop/winsock/getting-started-with-winsock
* ------------------------------------------------------------------------
*/

#include "../inc/server.h"

#undef UNICODE

int getIP(void)
{

    /*struct ifaddrs *id;
    int val;
    val = getifaddrs(&id);
    printf("Network Interface Name :- %s\n",id->ifa_name);
    printf("Network Address of %s :- %d\n",id->ifa_name,id->ifa_addr);
    printf("Network Data :- %d \n",id->ifa_data);
    printf("Socket Data : -%c\n",id->ifa_addr->sa_data);
    return 0;*/

    #ifdef _WIN32
        system("ipconfig | findstr IPv4");  //Credit: https://stackoverflow.com/questions/49216004/c-programming-getting-windows-ip-address
    #endif

    #ifdef linux
        //Credit: http://man7.org/linux/man-pages/man3/getifaddrs.3.html
        /*struct ifaddrs *ifaddr, *ifa;
        int family, s, n;
        char host[NI_MAXHOST];

        if (getifaddrs(&ifaddr) == -1) {
           perror("getifaddrs");
           exit(EXIT_FAILURE);
        }*/

        /* Walk through linked list, maintaining head pointer so we
          can free list later */

        /*for (ifa = ifaddr, n = 0; ifa != NULL; ifa = ifa->ifa_next, n++) {
           if (ifa->ifa_addr == NULL)
               continue;

           family = ifa->ifa_addr->sa_family;

           /* For an AF_INET* interface address, display the address */

           /*if (family == AF_INET || family == AF_INET6) {
               s = getnameinfo(ifa->ifa_addr,
                       (family == AF_INET) ? sizeof(struct sockaddr_in) :
                                             sizeof(struct sockaddr_in6),
                       host, NI_MAXHOST,
                       NULL, 0, NI_NUMERICHOST);
               if (s != 0) {
                   printf("getnameinfo() failed: %s\n", gai_strerror(s));
                   exit(EXIT_FAILURE);
               }

               printf("\t\taddress: <%s>\n", host);
           } 
        }

        freeifaddrs(ifaddr);
        exit(EXIT_SUCCESS);*/

        //Credit: https://forgetcode.com/c/1483-program-to-get-the-ip-address
        struct ifaddrs *id;
        int val;
        val = getifaddrs(&id);
        printf("Network Interface Name :- %s\n",id->ifa_name);
        printf("Network Address of %s :- %d\n",id->ifa_name,id->ifa_addr);
    #endif

    return 0;
}



/*  
*   Function Name   : runServer(void)
*                   :
*   Description     : This function does most of the coordinating and calls the shots.
*                   : The basic flow of things is, create and bind to a socket, listen on that socket,
*                   : create a thread for each new client and lastly join threads, close sockets and leave.
*                   : 
*   Parameters      : N/A
*                   :
*   Returns         : int retCode: the return value indicating the success or failure of the function
*/
int runServer()
{
	int success = FAILURE;
    int server_socket = 0;        //This is the socket used by the server

    createSocket(server_socket);

    /*
    * start listening on the socket
    */
    if (listen (server_socket, 10) < 0) 
    {
        printf ("[SERVER] : listen() - FAILED.\n");        
        success = FAILURE;
    }

    closeSocket(server_socket);

	return success;
}



int createSocket(int server_socket)
{
    int result = 0;
    int successState = FAILURE;

    #ifdef _WIN32
        WSADATA wsaData;
        result = WSAStartup(MAKEWORD(2,2), &wsaData);

        if (result != 0) 
        {
            printf("WSAStartup failed with error: %d\n", result);
            successState = FAILURE;
        }
        else
        {
            successState = SUCCESS;
        }
    #endif

    #ifdef linux

    #endif

    //set up a new socket and bind to it
    newSocket(&server_socket);

    return successState;
}

int closeSocket(int server_socket)
{
    int retCode = 0;

    #ifdef _WIN32
        // cleanup
        closesocket(server_socket);
        retCode = WSACleanup();
    #endif

    #ifdef linux

    #endif

        return retCode;
}

int __cdecl windowsSockets(void) 
{
    WSADATA wsaData;
    int iResult;

    SOCKET ListenSocket = INVALID_SOCKET;
    SOCKET ClientSocket = INVALID_SOCKET;

    struct addrinfo *result = NULL;
    struct addrinfo hints;

    int iSendResult;
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;
    
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if ( iResult != 0 ) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Create a SOCKET for connecting to server
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    // Setup the TCP listening socket
    iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(result);

    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    // Accept a client socket
    ClientSocket = accept(ListenSocket, NULL, NULL);
    if (ClientSocket == INVALID_SOCKET) {
        printf("accept failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    // No longer need server socket
    closesocket(ListenSocket);

    // Receive until the peer shuts down the connection
    do {

        iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0) {
            printf("Bytes received: %d\n", iResult);

        // Echo the buffer back to the sender
            iSendResult = send( ClientSocket, recvbuf, iResult, 0 );
            if (iSendResult == SOCKET_ERROR) {
                printf("send failed with error: %d\n", WSAGetLastError());
                closesocket(ClientSocket);
                WSACleanup();
                return 1;
            }
            printf("Bytes sent: %d\n", iSendResult);
        }
        else if (iResult == 0)
            printf("Connection closing...\n");
        else  {
            printf("recv failed with error: %d\n", WSAGetLastError());
            closesocket(ClientSocket);
            WSACleanup();
            return 1;
        }

    } while (iResult > 0);

    // shutdown the connection since we're done
    iResult = shutdown(ClientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ClientSocket);
        WSACleanup();
        return 1;
    }

    // cleanup
    closesocket(ClientSocket);
    WSACleanup();

    return 0;
}














/*  
*   Function Name   : void *socketThread(void *clientSocket)
*                   :
*   Description     : This function is a separate thread that is created for each client. It checks for incoming messages
*                   : from the client and then calls a function to broadcast that message to all other clients.
*                   : 
*   Parameters      : void *clientSocket : A void pointer to the struct that has all the client and server information
*                   : as well as the struct used in our communication protocol.
*                   :
*   Returns         : int retCode: the return value indicating the success or failure of the function
*/
void *socketThread(void *clientSocket)
{
  dataStruct* clientInfo = (dataStruct*) clientSocket;
  int clSocket = clientInfo->client_socket;

  //read in the message from the client
  read (clSocket, &clientInfo->clientMessage, sizeof(clientInfo->clientMessage));

  clientInfo->client_socket = clSocket;
  int done = 1;

  //loop till client enters >>bye<<
  while((done = strcmp(clientInfo->clientMessage.message,">>bye<<")) != 0)
  {
    
    
    
    //read in the message from the client
    read (clSocket, &clientInfo->clientMessage, sizeof(clientInfo->clientMessage));
    
  }
  
    //shut server down
    /*close(clSocket);
    close (clientInfo->server_socket);*/
    
  return 0;
}



/*  
*   Function Name   : newSocket(int* server_socket)
*                   :
*   Description     : This function sets up and binds to a socket. If theres is any problems doing this, 
*                   : an error is printed to the screen.
*                   : 
*   Parameters      : int* server_socket : The socket that is about to be set up. It will be used by the server.
*                   :
*   Returns         : int retCode: the return value indicating the success or failure of the function
*/
int newSocket(int* server_socket)
{
    int retCode = 0;        //the return value indicating the success or failure of the function

#ifdef linux
    struct sockaddr_in server_addr;     //A struct used for the socket information.

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
#endif

    return retCode;
} //end newSocket function



/*  
*   Function Name   : int monitorClients(void *clientSocket)
*                   :
*   Description     : This function looks for new sockets that indicate a client wants to attach to the server.
*                   : It loops till the number of clients decrees to zero or it get killed. After picking up a
*                   : new client, it creates a thread for that client so the server can handle messages from it.
*                   : 
*   Parameters      : void *infoStruct : This is the struct that has all the data for the client and server.
*                   : It also holds another struct that is used for sending messages back and forth.
*                   :
*   Returns         : int retCode: the return value indicating the success or failure of the function
*/
int readClient(dataStruct *infoStruct)
{
    dataStruct* clientInfo  = (dataStruct*) infoStruct;     //A pointer to the struct that has all the client and server info.
    int     retCode         = 0;                            //The return value indicating the success or failure of the function.
    int     client_socket   = 0;                            //The client's socket, set by the accept call.
    int     client_len      = 0;                            //The size of the client_addr struct.
    int     server_socket   = clientInfo->server_socket;    //The server's socket.
    struct  sockaddr_in client_addr;                        //Struct with details about client socket.
    
    /*
    * accept a packet from the client.
    * this is a blocking operation.
    */
    client_len = sizeof (client_addr);
    if ((client_socket = accept (server_socket,(struct sockaddr *)&client_addr, &client_len)) < 0) 
    {
          printf ("[SERVER] : accept() FAILED\n");
          fflush(stdout);   
    }

    return retCode;
} //end monitorClients function
