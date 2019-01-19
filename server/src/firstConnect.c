



#include "../inc/server.h"

int connectonDetails(socketInfo *benchMarkConnection)
{
	int status = SUCCESS;
    socketInfo* socketDetails = (socketInfo*) benchMarkConnection;

    #ifdef _WIN32
        SOCKET connectInfoSocket = 0;           //This is the socket used by the server to get information about the benchmarking socket
        SOCKET client_socket = 0;
    #endif
    #ifdef linux
        int connectInfoSocket = 0;              //This is the socket used by the server to get information about the benchmarking socket
        int client_socket = 0;
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
    if ((connectInfoSocket = socket (AF_INET, SOCK_STREAM, 0)) < 0) 
    {
        printf ("[SERVER] : socket() FAILED. \nErrno returned %i\n", errno);
        status = FAILURE;
    }
    else
    {
        //bind to socket
        memset (&server_addr, 0, sizeof (server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = htonl (INADDR_ANY);
        server_addr.sin_port = htons (DEFAULT_PORT);

        if (bind (connectInfoSocket, (struct sockaddr *)&server_addr, sizeof (server_addr)) < 0) 
        {
            #ifdef _WIN32
                // cleanup
            	printf ("[SERVER] : bind() FAILED. \nErrno returned %i, %s\nWSAGetLastError returned %d\n", errno, strerror(errno), WSAGetLastError());
                closesocket(connectInfoSocket);
                status = WSACleanup();
            #endif

            #ifdef linux
                //shut server down
            	printf ("[SERVER] : bind() FAILED. \nErrno returned %i, %s\n", errno, strerror(errno));
                close(connectInfoSocket);
            #endif
            status = FAILURE;
        }

    }

    /*
    * start listening on the socket
    */
    if (listen (connectInfoSocket, 10) < 0) 
    {
        printf ("[SERVER] : listen() - FAILED.\n");     
        status = FAILURE;
        #ifdef _WIN32
	        // cleanup
	        closesocket(connectInfoSocket);
	        status = WSACleanup();
	    #endif

	    #ifdef linux
	        //shut server down
	        close(connectInfoSocket);
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

        int len = sizeof (server_addr);
        if ((client_socket = accept (connectInfoSocket,(struct sockaddr *)&server_addr, &len)) < 0) 
        {
              printf ("[SERVER] : accept() FAILED\n");
              fflush(stdout);   
        }

        #ifdef DEBUG
            printf("%s\n", "Client accepted");
        #endif

        char buf[BUFSIZ] = {'\0'};

    	//get socket type, port, block size, number of blocks
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

     #ifdef _WIN32
        // cleanup
        closesocket(connectInfoSocket);
        status = WSACleanup();
    #endif

    #ifdef linux
        //shut server down
        close(connectInfoSocket);
    #endif

    return status;
}