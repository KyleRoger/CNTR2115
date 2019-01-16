


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
	int totalSize = 0;
	unsigned int addr;
	struct sockaddr_in server;

#ifdef _WIN32
	WSADATA wsaData;
	SOCKET  conn_socket;
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
		if ((retval = WSAStartup(0x202, &wsaData)) != 0) {
			printf("WSAStartup failed with error\n");
			WSACleanup();
		}
		else
#endif
		{
			
			server.sin_family = AF_INET;
			server.sin_port = htons(port);
			server.sin_addr.s_addr = inet_addr(serverIP);

			conn_socket = socket(AF_INET, SOCK_STREAM, 0); 
			if (conn_socket < 0) 
			{
				printf("Client: Error Opening socket: Error\n");
#ifdef _WIN32				
				WSACleanup();
#endif
			}

			else
			{
				if (connect(conn_socket, (struct sockaddr*)&server, sizeof(server)) < 0) 
				{
					printf("connect() failed: %s \n", serverIP);
#ifdef _WIN32									
					WSACleanup();
#endif			
				}

/*				printf("%i\n", initialConnect.socketType);
				printf("%i\n", initialConnect.userPort);
				printf("%i\n", initialConnect.blockSize);
				printf("%i\n", initialConnect.numBlocks);*/
				/*strcpy(buf, initialConnect.socketType);
				strcat(buf, " ");
				strcat(buf, initialConnect.userPort);*/
				printf("buf is: %s\n", buf);
				retval = send(conn_socket, buf, sizeof(buf), 0);

#ifdef _WIN32
				closesocket(conn_socket);
				WSACleanup();
#endif


#ifdef _WIN32
				if ((retval = WSAStartup(0x202, &wsaData)) != 0) 
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
						conn_socket = socket(AF_INET, SOCK_STREAM, 0);
					}
					else 
					{
						conn_socket = socket(AF_INET, SOCK_DGRAM, 0);
					} 
					if (conn_socket < 0) 
					{
						printf("Client: Error Opening socket: Error\n");
#ifdef _WIN32				
						WSACleanup();
#endif
					}

					else
					{
						if (connect(conn_socket, (struct sockaddr*)&server, sizeof(server)) < 0) 
						{
							printf("connect() failed: %s \n", serverIP);
#ifdef _WIN32									
							WSACleanup();
#endif			
						}

						//char* block = malloc(blockSize);
						//char block[blockSize] = "";
					
						for(i =  0; i < numBlocks; i++)
						{

							//memset(block, '\0',blockSize);


							//block[0] = i;

							//printf("Block Size: %i\n", sizeof(block));

							
           						sprintf(block, "%d", i);
								retval = send(conn_socket, block, blockSize, 0);
								if (retval < 0) {
									printf("send() failed: error %d\n", i);
#ifdef _WIN32					
									WSACleanup();
#endif					
									break;
							}
						}
						//free(block);
#ifdef _WIN32
						closesocket(conn_socket);
						WSACleanup();
					}
#endif
				}
			}
		}
	}

	return 0;
}

