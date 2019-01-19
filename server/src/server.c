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
*				: Steps for creating windows TCP sockets on server.
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

int exitFlag = RUN;


/*  
*   Function Name   : getIP(void)
*                   :
*   Description     : This function gets the IP address of the computer and prints it to the screen.
*                   : 
*   Parameters      : N/A
*                   :
*   Returns         : N/A
*/
void getIP(void)
{
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
*                   : 
*                   : 
*   Parameters      : N/A
*                   :
*   Returns         : int status: the return value indicating the success or failure of the function
*/
int runServer(int argc, char *argv[])
{
	int status = SUCCESS;

    #ifdef _WIN32
        SOCKET connectInfoSocket = 0;           //This is the socket used by the server to get information about the benchmarking socket
        SOCKET benchMarkSocket = 0;             //The socket used for benchmark tests
    #endif
    #ifdef linux
        int connectInfoSocket = 0;              //This is the socket used by the server to get information about the benchmarking socket
        int benchMarkSocket = 0;                //The socket used for benchmark tests
    #endif

    int connectInfoPort = DEFAULT_PORT;
    int socketType = DEFAULT_SOCK_TYPE;
    socketInfo benchMarkConnection;             //Struct used to store connection details for the benchmark socket
  
    getIP();                                    //Display the server ipv4 address

    //check startup arguments and fireup socket connections
    if ((connectInfoPort = parseCmdLine(argc, argv)) != INVALID_PARMS)
    {

        status = connectonDetails(&benchMarkConnection);

        #ifdef DEBUG
            printf("\n%s\n", "Test connection details"); 
            printf("\tSocket is: %i\n", benchMarkConnection.socketType);
            printf("\tPort is: %i\n", benchMarkConnection.userPort);
            printf("\tBlock size is: %i\n", benchMarkConnection.blockSize);
            printf("\tNumber of blocks is: %i\n\n", benchMarkConnection.numBlocks);
        #endif           

        if (benchMarkConnection.socketType == SOCK_STREAM)
        {
            printf("TCP socket\n");
            readTCP(benchMarkConnection);
        }
        else if (benchMarkConnection.socketType == SOCK_DGRAM)
        {
            printf("UDP Socket\n");
            readUDP(benchMarkConnection);
        }
        else
        {
            printf("Not a valid socket type\n");
            status = FAILURE;
        }

       /* #ifdef _WIN32
            initSocket();
        #endif*/

        //set up a new TCP socket and bind to it
        /*if (newSocket(&connectInfoSocket, SOCK_STREAM, connectInfoPort) < 0)
        {
            printf("%s\n", "Failed to create and bind to socket");
            status = FAILURE;
        }
        else
        {*/
            // testType(&benchMarkConnection, connectInfoSocket);

            
        // }
    }
    else
    {
        status = FAILURE;
    }

    if (status != FAILURE)
    {
        while (exitFlag == RUN)
        {
            if(benchMarkConnection.socketType == SOCK_STREAM)   //TCP connection requested
            {

                //Create a socket for benmarking
                /*if (newSocket(&benchMarkSocket, benchMarkConnection.socketType, benchMarkConnection.userPort) < 0)
                {
                    printf("%s\n", "Failed to create and bind to socket");
                    status = FAILURE;
                }
                else
                {*/
                    /*
                    * start listening on the socket
                    */
                   /* if (listen (benchMarkSocket, 10) < 0) 
                    {
                        printf ("[SERVER] : listen() - FAILED.\n");     
                        status = FAILURE;
                    }
                    else
                    {
                        status = readClient(benchMarkSocket, benchMarkConnection.numBlocks, benchMarkConnection.blockSize, benchMarkConnection.socketType);
                    }
                }*/
            }
            else if (benchMarkConnection.socketType == SOCK_DGRAM)   //UDP connection requested
            {
                //Create a socket for benmarking
                if (newSocket(&benchMarkSocket, benchMarkConnection.socketType, benchMarkConnection.userPort) < 0)
                {
                    printf("%s\n", "Failed to create and bind to socket");
                    status = FAILURE;
                }
                else
                {
                    status = readClient(benchMarkSocket, benchMarkConnection.numBlocks, benchMarkConnection.blockSize, benchMarkConnection.socketType);
                }
            }
            else
            {
                //not a reconized socket type
                status = FAILURE;
                exitFlag = EXIT;
            }

            #ifdef _WIN32
                // cleanup
                closesocket(connectInfoSocket);
                status = WSACleanup();
            #endif

            #ifdef linux
                //shut server down
                close(connectInfoSocket);
            #endif

            exitFlag = EXIT;
        }
    }

	return status;
}



void testType(socketInfo *benchMarkConnection, int connectInfoSocket)
{

    socketInfo* socketDetails = (socketInfo*) benchMarkConnection;
    int     client_socket   = 0;                            //The client's socket, set by the accept call.
    int     client_len      = 0;                            //The size of the client_addr struct.
    struct  sockaddr_in client_addr;                        //Struct with details about client socket.
    //char buf[BUFSIZ];

    #ifdef DEBUG
    printf("%s\n", "Check for type of test connection");
    #endif

    /*
    * start listening on the socket
    */
    if (listen (connectInfoSocket, 10) < 0) 
    {
        printf ("[SERVER] : listen() - FAILED.\n");     
        close (connectInfoSocket);   
    }
    else
    {
        /*
        * accept a packet from the client.
        * this is a blocking operation.
        */
        #ifdef DEBUG
            printf("%s\n", "Block till client is accepted");
        #endif

        client_len = sizeof (client_addr);
        if ((client_socket = accept (connectInfoSocket,(struct sockaddr *)&client_addr, &client_len)) < 0) 
        {
              printf ("[SERVER] : accept() FAILED\n");
              fflush(stdout);   
        }

        #ifdef DEBUG
            printf("%s\n", "Client accepted");
        #endif

        //get socket type, port, block size, number of blocks
        char buf[BUFSIZ] = {'\0'};
        //read (client_socket, socketDetails, sizeof(socketDetails));
        #ifdef _WIN32
            if (recv (client_socket, (void*)&buf, sizeof(buf), 0) < 0)
            {
                printf ("[SERVER] : socket() recv FAILED. \nErrno returned %i\n", errno);
                printf("recv failed with error: %d\n", WSAGetLastError());
            }
        #endif
        #ifdef linux
            if (read (client_socket, &buf, sizeof(buf)) < 0)
            {
                printf ("[SERVER] : socket() recv FAILED. \nErrno returned %i\n", errno);
            }
        #endif
       
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
        socketDetails->userPort = atoi(newString[1]);
        socketDetails->blockSize = atoi(newString[2]);
        socketDetails->numBlocks = atoi(newString[3]);

        fflush(stdout);
    }
}




int initSocket(void)
{

    int statusState = FAILURE;

    // Initialze winsock
    #ifdef _WIN32
        WSADATA wsaData;
        //result = WSAStartup(MAKEWORD(2,2), &wsaData);
        WORD ver = MAKEWORD(2, 2);

        int result = WSAStartup(ver, &wsaData);

        if (result != 0) 
        {
            printf("WSAStartup failed with error: %d\n", result);
            statusState = FAILURE;
        }
        else
        {
            #ifdef DEBUG
                printf("%s\n", "Winsock initialized"); 
            #endif

            statusState = SUCCESS;
        }
    #endif

    return statusState;
}






/*  
*   Function Name   : newSocket(int* server_socket)
*                   :
*   Description     : This function sets up and binds to a socket. If theres is any problems doing this, 
*                   : an error is printed to the screen.
*                   : 
*   Parameters      : int* server_socket : The socket that is about to be set up. It will be used by the server.
*                   :
*   Returns         : int retCode: the return value indicating the status or failure of the function
*/
int newSocket(int* server_socket, int sockType, int sockPort)
{
    int retCode = 0;                    //the return value indicating the status or failure of the function
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
            #ifdef _WIN32
            printf ("[SERVER] : bind() FAILED. \nErrno returned %i, %s\nWSAGetLastError returned %d\n", errno, strerror(errno), WSAGetLastError());
            #endif
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
*   Returns         : int retCode: the return value indicating the status or failure of the function
*/
int readClient(int benchMarkSocket, int numBlocks, int blockSize, int sockType)
{
    int     retCode         = 0;                //The return value indicating the status or failure of the function.
    int     client_socket   = 0;                //The client's socket, set by the accept call.
    int     client_len      = 0;                //The size of the client_addr struct.
    struct  sockaddr_in client_addr;            //Struct with details about client socket.
    char block[10000] = {'\0'};
    int sType = sockType;
    int num = numBlocks;
    int size = blockSize;
    char allBlocks[BUFSIZ][BUFSIZ];
    int recv_len = 0;

    /*
    * accept a packet from the client.
    * this is a blocking operation.
    */
    client_len = sizeof (client_addr);
    if (sType == SOCK_STREAM)
    {
        if ((client_socket = accept (benchMarkSocket,(struct sockaddr *)&client_addr, &client_len)) < 0) 
        {
              printf ("[SERVER] : accept() FAILED\n");
              fflush(stdout);   
              retCode = FAILURE;
        }


        #ifdef DEBUG
            printf("Number of blocks is:%i, Block size is: %i\n", numBlocks, blockSize);
            printf("\n%s\n", "benchmark socket accepted");
        #endif

        // Start timer
        float startTime = (float)clock()/CLOCKS_PER_SEC;
        
        //read the data from the socket
        for (int i = 0; i < num; i++)
        {    

            #ifdef _WIN32
                if (recv (client_socket, block, size, 0) < 0)
                {
                    printf ("[benchMarkSocket] : socket() recv FAILED. \nErrno returned %s\n", strerror(errno));
                    printf("recv failed with error: %d\n", WSAGetLastError());
                    closesocket(client_socket);
                    WSACleanup();
                }
            #endif
            #ifdef linux
                if (read (client_socket, block, size) < 0)
                {
                    printf ("[benchMarkSocket] : socket() recv FAILED. \nErrno returned %i\n", errno);
                }
            #endif

            strcpy(allBlocks[i], block);
            #ifdef DEBUG
                printf("Read: %s", block);
                printf(" Size is %zu\n", sizeof(block));
            #endif
        }

        float endTime = (float)clock()/CLOCKS_PER_SEC;
        float elapsedTime = endTime - startTime;
        printf("The measured time is: %f\n", elapsedTime);
    }
    else    //UDP socket
    {

      /*  SOCKET s;
    struct sockaddr_in server, si_other;
    int slen , recv_len;
    char buf[BUFLEN];
    WSADATA wsa;

    slen = sizeof(si_other) ;
    
    //Initialise winsock
    printf("\nInitialising Winsock...");
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        printf("Failed. Error Code : %d",WSAGetLastError());
        exit(EXIT_FAILURE);
    }
    printf("Initialised.\n");
    
    //Create a socket
    if((s = socket(AF_INET , SOCK_DGRAM , 0 )) == INVALID_SOCKET)
    {
        printf("Could not create socket : %d" , WSAGetLastError());
    }
    printf("Socket created.\n");
    
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( PORT );
    
    //Bind
    if( bind(s ,(struct sockaddr *)&server , sizeof(server)) == SOCKET_ERROR)
    {
        printf("Bind failed with error code : %d" , WSAGetLastError());
        exit(EXIT_FAILURE);
    }
    puts("Bind done");

    //keep listening for data
    while(1)
    {
        printf("Waiting for data...");
        fflush(stdout);
        
        //clear the buffer by filling null, it might have previously received data
        memset(buf,'\0', BUFLEN);
        
        //try to receive some data, this is a blocking call
        if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)) == SOCKET_ERROR)
        {
            printf("recvfrom() failed with error code : %d" , WSAGetLastError());
            exit(EXIT_FAILURE);
        }
        
        //print details of the client/peer and the data received
        printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
        printf("Data: %s\n" , buf);
        
        //now reply the client with the same data
        if (sendto(s, buf, recv_len, 0, (struct sockaddr*) &si_other, slen) == SOCKET_ERROR)
        {
            printf("sendto() failed with error code : %d" , WSAGetLastError());
            exit(EXIT_FAILURE);
        }
    }

    closesocket(s);
    WSACleanup();
    
    return 0;
*/
//********************************

        int retVal = 0;

        // Start timer
        float startTime = (float)clock()/CLOCKS_PER_SEC;

        //keep listening for data
       /* while(1)    // Credit: https://www.binarytides.com/udp-socket-programming-in-winsock/
        {*/
            #ifdef DEBUG
            printf("Waiting for data...");
            #endif
            fflush(stdout);
            
            //clear the buffer by filling null, it might have previously received data
            memset(block,'\0', size);
            
            //try to receive some data, this is a blocking call
            if ((recv_len = recvfrom(client_socket, block, size, MSG_WAITALL, (struct sockaddr *)&client_addr, &client_len)) < 0)
            {
                //printf("recvfrom() failed with error code : %d" , WSAGetLastError());
                //retCode = SOCKET_ERROR;
            }
            
            //print details of the client/peer and the data received
            #ifdef DEBUG
            printf("Received packet from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
            printf("Data: %s\n" , block);
            #endif
        //}
        /*while(exitFlag == RUN)
        {
            retVal = recvfrom(client_socket, block, size, 0, (struct sockaddr *)&client_addr, &client_len);
            printf("%i\n", retVal);
            #ifdef _WIN32
            Sleep(1000);
            #endif
            
        }*/

        //strcpy(allBlocks[i], block);
        #ifdef DEBUG
            printf("Read: %s\n", block);
        #endif
        float endTime = (float)clock()/CLOCKS_PER_SEC;
        float elapsedTime = endTime - startTime;
        printf("The measured time is: %f\n", elapsedTime);
    }

    return retCode;
} //end readClient function







/*  
*   Function Name   : parseCmdLine
*   Description     : This function takes in agruments from the command line and seperate them into useful parts.
*                   : It also does checks for invalid input. 
*                   : Then it saves the arguments status for later use before returning the outcome of checks made. 
*   Parameters      : int argc - number of command line arguments
*                   : char** argv - where the command line arguments are
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