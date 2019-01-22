/*
* -----------------------------------------------------------------------
* File			: server.c
* Project		: ispeed
* Author 		: Arie Kraayenbrink
* Editors		: Arie kraayenbrink
* Date 			: Jan. 8, 2019
* Description 	: This file has the function that starts and runs the server. It also parses the command line input to find the 
*               : starting port numer. The server ip address is also printed to the screen.
*               : The server first creates a TCP socket to find out what kind of connection is required for the benchmarking tests.  
*				: It then uses either TCP or UPD as specified on the requested port to do benchmarking of the speed of the choosen 
*               : socket type.
*               :
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

volatile int exitFlag = RUN;



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
*   Parameters      : int argc      : The number of command line arguments.
*                   : char *argv[]  : The command line arguments
*                   :
*   Returns         : int status    : the return value indicating the success or failure of the function
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
    
    while (exitFlag == RUN)
    {
        //check startup arguments and fireup socket connections
        if ((connectInfoPort = parseCmdLine(argc, argv)) != INVALID_PARMS)
        {

            status = connectonDetails(&benchMarkConnection);

            #ifdef DEBUG
                printf("Connect socket returned a status of: %d\n", status);
                printf("\n%s\n", "Test connection details"); 
                printf("\tSocket is: %i\n", benchMarkConnection.socketType);
                printf("\tPort is: %i\n", benchMarkConnection.userPort);
                printf("\tBlock size is: %i\n", benchMarkConnection.blockSize);
                printf("\tNumber of blocks is: %i\n\n", benchMarkConnection.numBlocks);
            #endif           
        }
        else
        {
            status = FAILURE;
        }

        if (status == SUCCESS)
        {
            if (benchMarkConnection.socketType == SOCK_STREAM)
            {
                #ifdef DEBUG
                printf("TCP socket\n");
                #endif
                status = readTCP(benchMarkConnection);
            }
            else if (benchMarkConnection.socketType == SOCK_DGRAM)
            {
                #ifdef DEBUG
                printf("UDP Socket\n");
                #endif
                status = readUDP(benchMarkConnection);
            }
            else
            {
                printf("Not a valid socket type\n");
                status = FAILURE;
            }
        }

        if (status == FAILURE)
        {
            exitFlag = EXIT;
        }
    }

	return status;
}



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