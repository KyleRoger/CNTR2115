


#include "../inc/client.h"


int runClient(int argc, char* argv[])
{
	socketInfo initialConnect;
	int port = 15000;
	int i =0;
	int socketType = 0;
	int userPort = 0;
	int numBlocks = 0;
	int blockSize = 0;
	char* block = NULL;
	char* serverIP = NULL;
	int retval = 0;
	char * buffer = NULL;
	char block1000[1000] = "";
	char block2000[2000] = "";
	char block5000[5000] = "";
	char block10000[10000] = "";
	char buf[BUFSIZ] = {'\0'};
	unsigned int addr;
	struct sockaddr_in server;

#ifdef _WIN32
	WSADATA wsaData;
	SOCKET  inital_socket, message_socket;
#endif
#ifdef _linux_
	int initial_socket = 0;
	int message_socket = 0;
#endif

	if (argc != 10)
	{
		printf("Not correct amount of Agruments");
	}
	else
	{
		for(i = 0; i < argc; i++)
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

		if(blockSize != 1000 && blockSize != 2000 && blockSize != 5000 && blockSize != 10000)
		{
			printf("Invalid Number of Blocks\n");
			return 0;
		}
		else
		{
			if(blockSize == 1000)
			{
				block = block1000;
			}
			else if( blockSize == 2000)
			{
				block = block2000;
			}
			else if( blockSize == 5000)
			{
				block = block5000;
			}
			else if( blockSize == 10000)
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
			
			server.sin_family = AF_INET;
			server.sin_port = htons(port);
			server.sin_addr.s_addr = inet_addr(serverIP);

			inital_socket = socket(AF_INET, SOCK_STREAM, 0); 
			if (inital_socket < 0) 
			{
				printf("Client: Error Opening socket: Error\n");
#ifdef _WIN32				
				WSACleanup();
#endif
			}

			else
			{
				if (connect(inital_socket, (struct sockaddr*)&server, sizeof(server)) < 0) 
				{
					printf("connect() failed: %s \n", serverIP);
#ifdef _WIN32									
					WSACleanup();
#endif			
				}
				printf("buf is: %s\n", buf);
				retval = send(inital_socket, buf, sizeof(buf), 0);
				if (retval < 0) 
				{
					printf("send() failed: error %d\n", i);
#ifdef _WIN32					
					WSACleanup();
#endif					
					return 0;
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
					
					server.sin_family = AF_INET;
					server.sin_port = htons(userPort);
					server.sin_addr.s_addr = inet_addr(serverIP);

					if(socketType == SOCK_STREAM)
					{
						message_socket = socket(AF_INET, SOCK_STREAM, 0);
					}
					else 
					{
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
						if (connect(message_socket, (struct sockaddr*)&server, sizeof(server)) < 0) 
						{
							printf("connect() failed: %s \n", serverIP);
#ifdef _WIN32									
							WSACleanup();
#endif			
						}
					
						for(i =  0; i < numBlocks; i++)
						{
           					sprintf(block, "%d", i);
           					printf("%s", block);
							retval = send(message_socket, block, blockSize, 0);
							if (retval < 0) 
							{
								printf("send() failed: error %d\n", i);
#ifdef _WIN32					
								WSACleanup();
#endif					
								break;
							}
						}
						strcpy(buf, "Bye");
						retval = send(inital_socket, buf, sizeof(buf), 0);
						if (retval < 0) 
								{
									printf("send() failed: error %d\n", i);
#ifdef _WIN32					
									WSACleanup();
#endif					
									return 0;
								}
#ifdef _WIN32
						closesocket(message_socket);
						closesocket(inital_socket);
						WSACleanup();					
#endif
#ifdef _linux_
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


/*int GetConnection()
{

			server.sin_family = AF_INET;
			server.sin_port = htons(port);
			server.sin_addr.s_addr = inet_addr(serverIP);
}*/