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

static int numClients = 0;  //used to track the number of clients attached.
int fExitFlag = 0;


/*  
*   Function Name   : getIP(void)
*                   :
*   Description     : This function gets the IP address of the computer.
*                   : 
*   Parameters      : N/A
*                   :
*   Returns         : N/A
*/
void getIP(void)
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
        struct ifaddrs *ifaddr, *ifa;
        int family, s, n;
        char host[NI_MAXHOST];

        if (getifaddrs(&ifaddr) == -1) {
           perror("getifaddrs");
        }
        else
        {
            /* Walk through linked list, maintaining head pointer so we
              can free list later */

            for (ifa = ifaddr, n = 0; ifa != NULL; ifa = ifa->ifa_next, n++) {
               if (ifa->ifa_addr == NULL)
                   continue;

               family = ifa->ifa_addr->sa_family;

               /* For an AF_INET* interface address, display the address */

               if (family == AF_INET) {
                   s = getnameinfo(ifa->ifa_addr,
                           (family == AF_INET) ? sizeof(struct sockaddr_in) :
                                                 sizeof(struct sockaddr_in6),
                           host, NI_MAXHOST,
                           NULL, 0, NI_NUMERICHOST);
                   if (s != 0) {
                       printf("getnameinfo() failed: %s\n", gai_strerror(s));
                       break;
                   }

                   printf("address: <%s>\n", host);
               } 
            }

            freeifaddrs(ifaddr);
        }
    #endif
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
int runServer(int argc, char *argv[])
{
	int success = FAILURE;
    #ifdef _WIN32
        SOCKET serverSocket = 0;          //This is the socket used by the server
        SOCKET clientSocket = 0;          //the client socket ID
        SOCKET testSocket = 0;
    #endif
    #ifdef linux
        int serverSocket = 0;          //This is the socket used by the server
        int clientSocket = 0;          //the client socket ID
        int testSocket = 0;
    #endif
    int socketPort = 15000;
    int socketType = SOCK_STREAM;
    char buffer[BUFSIZ];            // used for accepting incoming command and also holding the command's response
    socketInfo testConnection;
    int bind_value;
    int fromlen;
    struct sockaddr_in local;
    struct sockaddr_in from;

    getIP();

    if ((socketPort = parseCmdLine(argc, argv)) != INVALID_PARMS)
    {
        
        #ifdef _WIN32
            initSocket();
        #endif

        //set up a new socket and bind to it
        newSocket(&serverSocket, socketType, socketPort);

        testType(&testConnection, serverSocket);

        #ifdef DEBUG
        printf("%s\n", "Test connection details"); 
        printf("Socket is: %i\n", testConnection.socketType);
        printf("Port is: %i\n", testConnection.port);
        printf("Block size is: %i\n", testConnection.blockSize);
        printf("Number of blocks is: %i\n", testConnection.numBlocks);
        #endif

        readClient();


        

        //check for clients
        //monitorClients(&clientInfo);

        #ifdef _WIN32
            // cleanup
            closesocket(serverSocket);
            success = WSACleanup();
        #endif

        #ifdef linux
            //shut server down
            close(serverSocket);
        #endif
    }

	return success;
}



void testType(socketInfo *testConnection, int serverSocket)
{

    socketInfo* socketDetails = (socketInfo*) testConnection;
    int     client_socket   = 0;                            //The client's socket, set by the accept call.
    int     client_len      = 0;                            //The size of the client_addr struct.
    struct  sockaddr_in client_addr;                        //Struct with details about client socket.
    //char buf[BUFSIZ];

    printf("%s\n", "Check for type of test connection");

    /*
    * start listening on the socket
    */
    if (listen (serverSocket, 10) < 0) 
    {
        printf ("[SERVER] : listen() - FAILED.\n");     
        close (serverSocket);   
    }
    else
    {
        /*
        * accept a packet from the client.
        * this is a blocking operation.
        */
        printf("%s\n", "Block till client is accepted");
        client_len = sizeof (client_addr);
        if ((client_socket = accept (serverSocket,(struct sockaddr *)&client_addr, &client_len)) < 0) 
        {
              printf ("[SERVER] : accept() FAILED\n");
              fflush(stdout);   
        }

        printf("%s\n", "Client accepted");

        //get socket type, port, block size, number of blocks
        //read (client_socket, buf, sizeof(buf));
        char buf[BUFSIZ] = {'\0'};
        recv (client_socket, buf, 1024, 0);
        printf("Read [%s] from client\n", buf);
        // Credit: https://www.w3resource.com/c-programming-exercises/string/c-string-exercise-31.php
        char newString[10][10]; 
        int j = 0;
        int ctr = 0;
     
        for(int i=0; i <= (strlen(buf)); i++)
        {
            // if space or NULL found, assign NULL into newString[ctr]
            if(buf[i] == ' ' || buf[i] == '\0')
            {
                newString[ctr][j]='\0';
                ctr++;  //for next word
                j=0;    //for next word, init index to 0
            }
            else
            {
                newString[ctr][j] = buf[i];
                j++;
            }
        }

        socketDetails->socketType = atoi(newString[0]);
        socketDetails->port = atoi(newString[1]);
        socketDetails->blockSize = atoi(newString[2]);
        socketDetails->numBlocks = atoi(newString[3]);

        fflush(stdout);
    }
}




int initSocket(void)
{

    int successState = FAILURE;

    // Initialze winsock
    #ifdef _WIN32
        WSADATA wsaData;
        //result = WSAStartup(MAKEWORD(2,2), &wsaData);
        WORD ver = MAKEWORD(2, 2);

        int result = WSAStartup(ver, &wsaData);

        if (result != 0) 
        {
            printf("WSAStartup failed with error: %d\n", result);
            successState = FAILURE;
        }
        else
        {
            printf("%s\n", "Winsock initialized"); 
            successState = SUCCESS;
        }
    #endif



/*
    // Create a socket
    #ifdef _WIN32
        SOCKET serverSocket = socket(AF_INET, socketType, 0);
    #endif

    #ifdef linux
        int serverSocket = socket(AF_INET, socketType, 0);
    #endif

    if (serverSocket == INVALID_SOCKET)
    {
        printf ("[SERVER] : socket() FAILED. \nErrno returned %i\n", errno);
        successState = FAILURE;
        #ifdef _WIN32
            WSACleanup();
        #endif
    }
    



    // Bind ip to socket
    socketaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(socketPort);
    //inet_pton(AF_INET, ipAddress, &hint.sin_addr);
    //inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);
    hint.sin_addr.s_un.s_addr = INADDR_ANY;

    bind (serverSocket, (sockaddr*)&hint, sizeof(hint));

    // Tell winsock the socket is for listening
    listen(serverSocket, SOMAXCONN);

    // Wait for a connection 
    sockaddr_in client;
    int clientSize = sizeof(client);
    //socklen_t clientSize = sizeof(client);

    socket clientSocket = accept (serverSocket, (sockaddr*)&client, &clientSize);

    if (clientSocket == INVALID_SOCKET)
    {

    }

    char host[NI_MAXHOST]       // client's remote name
    char service[NI_MAXHOST];   // Servic (i.e. port) the client is connected on

    ZeroMemory(host, NI_MAXHOST); 
    ZeroMemory(service, NI_MAXHOST);

    if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
    {
        printf("%s %i\n", "Connected on port: ", service);
    }
    else
    {
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        printf("%s %i\n", "Connected on post: ", ntohs(client.sin_port));
    }
    // Close listening socket
    closesocket(serverSocket);

    // While loop: accept messages from client

    // close the socket

    // Shutdown winsock*/


    

    #ifdef linux

    #endif

    return successState;
}

int closeSocket(int clSocket)
{
    int retCode = 0;

    #ifdef _WIN32
        // cleanup
        closesocket(clSocket);
    #endif

    #ifdef linux
        //shut server down
        close(clSocket);
    #endif

    return retCode;
}

#ifdef _WIN32
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
#endif



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
int monitorClients(dataStruct *infoStruct)
{
    dataStruct* clientInfo  = (dataStruct*) infoStruct;     //A pointer to the struct that has all the client and server info.
    int     retCode         = 0;                            //The return value indicating the success or failure of the function.
    int     client_socket   = 0;                            //The client's socket, set by the accept call.
    int     client_len      = 0;                            //The size of the client_addr struct.
    int     server_socket   = clientInfo->server_socket;    //The server's socket.
    struct  sockaddr_in client_addr;                        //Struct with details about client socket.

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
        }

        //track clients
        numClients++;
        clientInfo->client_socket = client_socket;

        printf("%s\n", "Client connected");
                    
    } 
    else
    {
        printf ("Error, reached max number of supported clients.\n");
    }

    int clSocket = clientInfo->client_socket;

    char buffer[BUFSIZ];
    //read in the message from the client
    //read (clSocket, &clientInfo->clientMessage, sizeof(clientInfo->clientMessage));
    printf("%s\n", "About to read from client");
    recv (clSocket, buffer, 1024, 0);
    buffer[1023] = '\0';    // ensure null termination!

    //read (clSocket, buffer, sizeof(buffer));

    printf("read: %s\n", buffer);

    clientInfo->client_socket = clSocket;
    int done = 1;

    //loop till client enters >>bye<<
    while((done = strcmp(clientInfo->clientMessage.message,">>bye<<")) != 0)
    {
        
    //read in the message from the client
    read (clSocket, &clientInfo->clientMessage, sizeof(clientInfo->clientMessage));

    }

    return retCode;
} //end monitorClients function



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
int newSocket(int* server_socket, int sockType, int sockPort)
{
    int retCode = 0;                    //the return value indicating the success or failure of the function
    struct sockaddr_in server_addr;     //A struct used for the socket information.

    //set up socket
    if ((*server_socket = socket (AF_INET, sockType, 0)) < 0) 
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
        server_addr.sin_port = htons (sockPort);

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
    

    if (testConnection.socketType == SOCK_STREAM)
    {
        /*
        * start listening on the socket
        */
        if (listen (serverSocket, 10) < 0) 
        {
            printf ("[SERVER] : listen() - FAILED.\n");     
            close (serverSocket);   
            success = FAILURE;
        }
    }

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



/*  
*   Function Name   : parseCmdLine
*   Description     : This function takes in agruments from the command line and seperate them into useful parts.
*                   : It also does checks for invalid input. If no input file is named, stdin is assumed. The
*                   : same for output, if it wasn't provided, use stdout. Also there are two types of file outputs
*                   : - srec or asm. Only one is used at a time.
*                   : Then it saves the arguments status for later use before returning the outcome of checks made. 
*   Parameters      : int argc - number of command line arguments
*                   : char** argv - where the command line arguments are
*                   : char* inputFileName - name of the file provided for input at the command line.
*                   : char* outputFileName - name of the file provided at the command line for output.
*   Returns         : int retCode - type of input, valid or otherwise, provided as arguments in the command line.
*/
int parseCmdLine(int argc, char** argv)
{

    int retCode = 0; //return code to let calling function know what to do.
    
    if (argc != 3)
    {
        showHelp(argv); //print help and quit
        retCode = INVALID_PARMS;
    }
    else if (argv[1][0] == '-' && argv[1][1] == 'p' && argv[1][2] == '\0') //look for a -p switch in the second parmeter.
    {           
        retCode = atoi (argv[2]);;
    }
    else
    {
        showHelp(argv); //print help and quit
        retCode = INVALID_PARMS;
    }

    return retCode;
} //end parseCmdLine function



/*  
*   Function Name   : showHelp
*   Description     : Display a help message to the screen.
*                   : Currently this function only deals with one help message
*                   : This message is basically just a quck usage statement.
*                   :
*   Parameters      : char** argv - what the command line arguments are.
*                   : int whichHelp - what message should be printed out to the screen.
*                   : 
*   Returns         : N/A
*/
void showHelp(char** argv)
{
    //This is basically just a quck usage statement.
    //show PROGRAM useage
    printf("Usage:\t%s <arguments>\n", argv[0]);
    printf("\t\t\t-p <port>\t: the port number for socket communication\n");

} //end showHelp function