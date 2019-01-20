

#include "../inc/server.h"


extern int exitFlag;

int readTCP(socketInfo benchMarkConnection)
{
	int     status = SUCCESS;                //The return value indicating the status or failure of the function.
    int     server_len      = 0;                //The size of the client_addr struct.
    char block[BUFLEN] = {'\0'};
    int sType = benchMarkConnection.socketType;
    int num = benchMarkConnection.numBlocks;
    int size = benchMarkConnection.blockSize;
    int port = benchMarkConnection.userPort;
    char allBlocks[10][BUFLEN];
    int recv_len = 0;
    char *blocks = (char*)malloc(num * size);

    // blocks[0] = 5;
    *(blocks + 1) = 5;


	#ifdef _WIN32
        SOCKET benchMarkSocket = 0;           	//This is the socket used by the server to get information about the benchmarking socket
        SOCKET client_socket = 0;				//The client's socket, set by the accept call.
    #endif
    #ifdef linux
        int benchMarkSocket = 0;              	//This is the socket used by the server to get information about the benchmarking socket
        int client_socket = 0;					//The client's socket, set by the accept call.
    #endif

 	// Initialze winsock
    #ifdef _WIN32
        WSADATA wsaData;
        //result = WSAStartup(MAKEWORD(2,2), &wsaData);
        WORD ver = MAKEWORD(2, 2);

        int result = WSAStartup(ver, &wsaData);

        if (result != 0) 
        {
            printf("WSAStartup failed with error: %d\n", result);
            status = FAILURE;
        }
        else
        {
            #ifdef DEBUG
                printf("%s\n", "Winsock initialized"); 
            #endif

            status = SUCCESS;
        }
    #endif

    struct sockaddr_in server_addr;     //A struct used for the socket information.

    //set up socket
    if ((benchMarkSocket = socket (AF_INET, SOCK_STREAM, 0)) < 0) 
    {
        printf ("[SERVER] : socket() FAILED. \nErrno returned %i\n", errno);
        status = FAILURE;
    }
    else
    {

        memset (&server_addr, 0, sizeof (server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = htonl (INADDR_ANY);
        server_addr.sin_port = htons (port);

        //bind to socket
        if (bind (benchMarkSocket, (struct sockaddr *)&server_addr, sizeof (server_addr)) < 0) 
        {
            #ifdef _WIN32
                // cleanup
            	printf ("[SERVER] : bind() FAILED. \nErrno returned %i, %s\nWSAGetLastError returned %d\n", errno, strerror(errno), WSAGetLastError());
                closesocket(benchMarkSocket);
                status = WSACleanup();
            #endif

            #ifdef linux
                //shut server down
            	printf ("[SERVER] : bind() FAILED. \nErrno returned %i, %s\n", errno, strerror(errno));
                close(benchMarkSocket);
            #endif
            status = FAILURE;
        }

    }

    /*
    * start listening on the socket
    */
    if (listen (benchMarkSocket, 10) < 0) 
    {
        printf ("[SERVER] : listen() - FAILED.\n");     
        status = FAILURE;
        #ifdef _WIN32
	        // cleanup
	        closesocket(benchMarkSocket);
	        status = WSACleanup();
	    #endif

	    #ifdef linux
	        //shut server down
	        close(benchMarkSocket);
	    #endif
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
	    /*
	    * accept a packet from the client.
	    * this is a blocking operation.
	    */
	    server_len = sizeof (server_addr);

	    if ((client_socket = accept (benchMarkSocket,(struct sockaddr *)&server_addr, &server_len)) < 0) 
	    {
	          printf ("[SERVER] : accept() FAILED\n");
	          fflush(stdout);   
	          status = FAILURE;
	    }


	    #ifdef DEBUG
	        printf("Number of blocks is:%i, Block size is: %i\n", num, size);
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

	printf("\nSocket test report\n");
	printf("*****************************************************\n");

	//check that all blocks are in order	
    for (int i = 0; i < num; i++)
    {
        if (atoi(allBlocks[i]) + 1 != atoi(allBlocks[i + 1]))
        {
            printf("Block [%i] out of order\n", atoi(allBlocks[i]));
        	printf("\tallBlocks[i] is: %i, allBlocks[i + 1] is: %i\n", atoi(allBlocks[i]), atoi(allBlocks[i + 1]));
        }

    }

    int notFound[BUFLEN] = {0};

	//check that all blocks are pressent
    for (int i = 0; i < num; i++)
    {
    	for (int j = 0; j < num; j++)
    	{
    		if (atoi(allBlocks[j]) == i)
    		{
    			continue;
    		}
    		else
    		{
    			notFound[i] = 1;
    		}    		
    	}
    }

    // check for missing blocks
    for (int i = 0; i < num; i++)
    {
    	if (notFound[i] == 1)
    	{
    		printf("Block [%i] was not found.\n", i);
    	}
    }  

	printf("*****************************************************\n");

	 #ifdef _WIN32
        // cleanup
        closesocket(benchMarkSocket);
        status = WSACleanup();
    #endif

    #ifdef linux
        //shut server down
        close(benchMarkSocket);
    #endif

    free(blocks);
        
	return status;
}



/*  
*   Function Name   : readUDP
*   Description     : This function takes in an array and checks the data is as expected.
*                   : Then it prints the report to the screen.
*   Parameters      : char blockData[][] - The incomeing data from the sockets.
*                   : int numBlocks - the number of expected blocks sent over the socket.
*   Returns         : N/A
*/
int readUDP(socketInfo benchMarkConnection)
{
	int status = SUCCESS;                	//The return value indicating the status or failure of the function.
    char block[BUFLEN] = {'\0'};
    int sType = benchMarkConnection.socketType;
    int num = benchMarkConnection.numBlocks;
    int size = benchMarkConnection.blockSize;
    int port = benchMarkConnection.userPort;
    char allBlocks[100][BUFLEN];
    int recv_len = 0;
    struct sockaddr_in server_addr;     //A struct used for the socket information.
    struct sockaddr_in client_addr;
    int len = sizeof(client_addr);

/* 	if (size == 1000)
 	{
    	char allBlocks[BUFLEN][1000]; 		
 	}
 	else if (size == 2000)
 	{
    	char allBlocks[BUFLEN][2000]; 		
 	}
 	else if (size == 5000)
 	{
    	char allBlocks[BUFLEN][5000]; 		
 	}
 	else
 	{
    	char allBlocks[BUFSIZ][10000]; 		
 	}*/

	#ifdef _WIN32
        SOCKET benchMarkSocket = 0;           	//This is the socket used by the server to get information about the benchmarking socket
    #endif
    #ifdef linux
        int benchMarkSocket = 0;              	//This is the socket used by the server to get information about the benchmarking socket
    #endif

 	// Initialze winsock
    #ifdef _WIN32
        WSADATA wsaData;
        //result = WSAStartup(MAKEWORD(2,2), &wsaData);
        WORD ver = MAKEWORD(2, 2);

        int result = WSAStartup(ver, &wsaData);

        if (result != 0) 
        {
            printf("WSAStartup failed with error: %d\n", result);
            status = FAILURE;
        }
        else
        {
            #ifdef DEBUG
                printf("%s\n", "Winsock initialized"); 
            #endif

            status = SUCCESS;
        }
    #endif

    //set up socket
    if ((benchMarkSocket = socket (AF_INET, SOCK_DGRAM, 0)) < 0) 
    {
        printf ("[SERVER] : socket() FAILED. \nErrno returned %i\n", errno);
        status = FAILURE;
    }
    else
    {

        memset (&server_addr, 0, sizeof (server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = htonl (INADDR_ANY);
        server_addr.sin_port = htons (port);

        //bind to socket
        if (bind (benchMarkSocket, (struct sockaddr *)&server_addr, sizeof (server_addr)) < 0) 
        {
            #ifdef _WIN32
                // cleanup
            	printf ("[SERVER] : bind() FAILED. \nErrno returned %i, %s\nWSAGetLastError returned %d\n", errno, strerror(errno), WSAGetLastError());
                closesocket(benchMarkSocket);
                status = WSACleanup();
            #endif

            #ifdef linux
                //shut server down
            	printf ("[SERVER] : bind() FAILED. \nErrno returned %i, %s\n", errno, strerror(errno));
                close(benchMarkSocket);
            #endif
            status = FAILURE;
        }
    }

    #ifdef DEBUG
        printf("Number of blocks is:%i, Block size is: %i\n", num, size);
        printf("\n%s\n", "benchmark socket accepted");
    #endif

    // Start timer
    float startTime = (float)clock()/CLOCKS_PER_SEC;
    
    //read the data from the socket
   /* for (int i = 0; i < num; i++)
    {    

        #ifdef _WIN32
            if (recv (client_socket, block, size, 0) < 0)
            {
                printf ("[benchMarkSocket] : socket() recv FAILED. \nErrno returned %s\n", strerror(errno));
                printf("recv failed with error: %d\n", WSAGetLastError());
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
    }*/
	//while(exitFlag == RUN)
    for (int i = 0; i < num; i++)
	{
		printf("Waiting for data...");
		fflush(stdout);
		
		//clear the buffer by filling null, it might have previously received data
		memset(block,'\0', BUFLEN);
		
		//try to receive some data, this is a blocking call
		#ifdef _WIN32
			if ((recv_len = recvfrom(benchMarkSocket, block, size, 0, (struct sockaddr *) &client_addr, &len)) == SOCKET_ERROR)
			{
				printf("recvfrom() failed with error code : %d" , WSAGetLastError());
				status = FAILURE;
			}
		#endif
		#ifdef linux
			if ((recv_len = recvfrom(benchMarkSocket, block, size, 0, (struct sockaddr *) &client_addr, &len)) < 0)
			{
				printf("recvfrom() failed with error : %d" , recv_len);
				status = FAILURE;
			}
		#endif
		
		//print details of the client/peer and the data received
		printf("Received packet \n");
		printf("Data: %s\n" , block);
		strcpy(allBlocks[i], block);
        #ifdef DEBUG
            printf("Read: %s", block);
            printf(" Size is %zu\n", sizeof(block));
        #endif		
	}


    float endTime = (float)clock()/CLOCKS_PER_SEC;
    float elapsedTime = endTime - startTime;
    printf("The measured time is: %f\n", elapsedTime);
	
	 #ifdef _WIN32
        // cleanup
        closesocket(benchMarkSocket);
        status = WSACleanup();
    #endif

    #ifdef linux
        //shut server down
        close(benchMarkSocket);
    #endif

	return status;
}
