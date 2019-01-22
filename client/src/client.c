
/*
* -----------------------------------------------------------------------
* File          : server.c
* Project       : ispeed
* Author        : Kyle Horsley
* Editors       : Kyle Horsley
* Date          : Jan. 8, 2019
* Description   : This file has the function that starts and runs the client. It also parses the command line input to find the 
*               : starting port numer. The server ip address is also printed to the screen.
*               : The server first creates a TCP socket to find out what kind of connection is required for the benchmarking tests.  
*               : It then uses either TCP or UPD as specified on the requested port to do benchmarking of the speed of the choosen 
*               : socket type.
* Credit        : https://docs.microsoft.com/en-us/windows/desktop/winsock/getting-started-with-winsock
* ------------------------------------------------------------------------
*/


#include "../inc/client.h"


int runClient(int argc, char* argv[])
{
	int port = DEFAULT_PORT;
	int i =0;
	int socketType = 0;
	int userPort = 0;
	int numBlocks = 0;
	int blockSize = 0;
	char* block =  NULL;
	char* serverIP = NULL;
	int retval = 0;
	char * buffer = NULL;
	char block1000[1000] = "";
	char block2000[2000] = "";
	char block5000[5000] =  "";
	char block10000[10000] = "";
	char buf[BUFSIZ] = {'\0'};
	unsigned int addr;
	struct sockaddr_in server;
	int len = sizeof(server);

#ifdef _WIN32
	WSADATA wsaData;
	SOCKET  initial_socket, message_socket;
#endif
#ifdef linux
	int initial_socket = 0;
	int message_socket = 0;
#endif

	//Check if the correct amount of arguments was passed
	if (argc != 10)
	{
		printf("Not correct amount of Agruments");
	}
	else
	{
		for(i = 0; i < argc; i++) //Loop and store all arguments in their appropriate places.
		{

			//Send sck type, port, block size, block number.
			if(strcmp(argv[i],"-TCP") == 0)
			{
				//strcpy(buf, SOCK_STREAM);
				// buf = (int)SOCK_STREAM;
				strcpy(buf, "1");
				strcat(buf, " ");
				socketType = SOCK_STREAM;
			}
			else if(strcmp(argv[i],"-UDP") == 0)
			{
				strcpy(buf, "2");
				strcat(buf, " ");
				socketType = SOCK_DGRAM;
			}
			else if(strcmp(argv[i], "-a") ==0)
			{
				serverIP = argv[i + 1];
			}
			else if(strcmp(argv[i],"-p") == 0)
			{
				strcat(buf, argv[i+1]);
				strcat(buf, " ");
				userPort = atoi(argv[i+1]);
			}
			else if(strcmp(argv[i],"-s") == 0)
			{
				strcat(buf, argv[i+1]);
				strcat(buf, " ");
				blockSize = atoi(argv[i+1]);
			}
			else if(strcmp(argv[i],"-n") == 0)
			{
				strcat(buf, argv[i+1]);
				numBlocks = atoi(argv[i+1]);
			}
		}

		if(blockSize != 1000 && blockSize != 2000 && blockSize != 5000 && blockSize != 10000) //Ensure a valid blocksize
		{
			printf("Invalid Number of Blocks\n");
			return 0;
		}
		else //Set the block to an array of the appropriate block size.
		{
			if(blockSize == 1000)
			{
				block = block1000;
			}
			else if(blockSize == 2000)
			{
				block = block2000;
			}
			else if(blockSize == 5000)
			{
				block = block5000;
			}
			else if(blockSize == 10000)
			{
				block = block10000;
			}
		}

#ifdef _WIN32
		if ((retval = WSAStartup(MAKEWORD(2,2), &wsaData)) != 0) 
		{
			printf("WSAStartup failed with error\n");
			WSACleanup();
		}
		else
#endif
		{
			//Initial socket connection
			server.sin_family = AF_INET;
			server.sin_port = htons(port);
			server.sin_addr.s_addr = inet_addr(serverIP);

			initial_socket = socket(AF_INET, SOCK_STREAM, 0); 
			if (initial_socket < 0) 
			{
				printf("Client: Error Opening socket: Error\n");
#ifdef _WIN32				
				WSACleanup();
#endif
			}

			else
			{
				//Connect to a socket
				if (connect(initial_socket, (struct sockaddr*)&server, sizeof(server)) < 0) 
				{
					printf("connect() failed: %s \n", serverIP);
#ifdef _WIN32									
					WSACleanup();
#endif			
				}
				//Send initial message containing command line data.
				retval = send(initial_socket, buf, sizeof(buf), 0);
				if (retval < 0) 
				{
#ifdef _WIN32					

					printf("\nsend() failed with error code : %d\n" , WSAGetLastError());
#endif
					printf("send() failed: error %d\n", i);
#ifdef _WIN32					
					WSACleanup();
#endif					
					return 0;
				}

#ifdef _WIN32
				        // cleanup
				        closesocket(initial_socket);
				        WSACleanup();
#endif

#ifdef linux
				        //shut server down
				        close(initial_socket);
#endif
#ifdef _WIN32
				Sleep(1000);
#endif
#ifdef linux
				sleep(1);
#endif

#ifdef _WIN32
				if ((retval = WSAStartup(MAKEWORD(2,2), &wsaData)) != 0) //Reconnect
				{
					printf("WSAStartup failed with error\n");
					WSACleanup();
				}
				else
#endif
				{					
					server.sin_family = AF_INET;
					server.sin_port = htons(userPort);
					server.sin_addr.s_addr = inet_addr(serverIP);

					if(socketType == SOCK_STREAM)
					{
						printf("%s\n", "TCP socket connecting...");
						message_socket = socket(AF_INET, SOCK_STREAM, 0);
					}
					else 
					{
						printf("%s\n", "UDP socket connecting...");
						message_socket = socket(AF_INET, SOCK_DGRAM, 0);
					} 
					if (message_socket < 0) 
					{
						printf("Client: Error Opening socket: Error\n");
#ifdef _WIN32				
						WSACleanup();
#endif
					}

					else
					{
						if (connect(message_socket, (struct sockaddr*)&server, sizeof(server)) < 0)  //Connect back to socket
						{
							printf("Connect() failed to connect to server at ip: %s \n", serverIP);
#ifdef _WIN32									
							printf("\tConnect() failed with error code : %d\n" , WSAGetLastError());
							WSACleanup();
#endif			
						}
					
						for(i =  0; i < numBlocks; i++) //Loop the number of blocks given from the user/
						{
           					sprintf(block, "%d", i);

           					if(socketType == SOCK_STREAM) //If TCP, send 
           					{

								retval = send(message_socket, block, blockSize, 0);
							}
							else //Else linux, send differently.
							{
								retval = sendto (message_socket, block, strlen(block), 0, (struct sockaddr *)&server, len); 
							}

							if (retval < 0) 
							{
								printf("send() failed: error.\n\tI = %d\n", i);
#ifdef _WIN32					
								printf("\nsend() failed with error code : %d\n" , WSAGetLastError());
								WSACleanup();
#endif					
								break;
							}
							memset(block, 0, blockSize); //Clear the block
						}

						//empty message indicates we are done
						send(message_socket, "\0", 0, 0);

						#ifdef DEBUG
						printf("\n%s\n", "Finnished sending blocks, sending Bye");
						#endif

#ifdef _WIN32
						//Close all the sockets
						closesocket(message_socket);
						closesocket(initial_socket);
						WSACleanup();					
#endif
#ifdef linux
						close(message_socket);
						close(initial_socket);
#endif
					}
				}
			}
		}
	}

	return 0;
}

