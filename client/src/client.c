


#include "../inc/client.h"


int runClient(int argc, char* argv[])
{
	socketInfo initialConnect;
	int port = 15000;
	int i =0;
	char* serverIP = NULL;
	int retval = 0;
	char * buffer = NULL;
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
				initialConnect.socketType = SOCK_STREAM;
			}
			else if(strcmp(argv[i],"-UDP") == 0)
			{
				strcpy(buf, "2");
				strcat(buf, " ");
				initialConnect.socketType = SOCK_DGRAM;
			}
			else if(strcmp(argv[i], "-a") ==0)
			{
				serverIP = argv[i + 1];
			}
			else if(strcmp(argv[i],"-p") == 0)
			{
				strcat(buf, argv[i+1]);
				strcat(buf, " ");
				initialConnect.userPort = atoi(argv[i+1]);
			}
			else if(strcmp(argv[i],"-s") == 0)
			{
				strcat(buf, argv[i+1]);
				strcat(buf, " ");
				initialConnect.blockSize = atoi(argv[i+1]);
			}
			else if(strcmp(argv[i],"-n") == 0)
			{
				strcat(buf, argv[i+1]);
				initialConnect.numBlocks = atoi(argv[i+1]);
			}
		}

		if(initialConnect.blockSize != 1000 && initialConnect.blockSize != 2000 && initialConnect.blockSize != 5000 && initialConnect.blockSize != 10000)
		{
			printf("Invalid Number of Blocks\n");
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

				printf("%i\n", initialConnect.socketType);
				printf("%i\n", initialConnect.userPort);
				printf("%i\n", initialConnect.blockSize);
				printf("%i\n", initialConnect.numBlocks);
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
					server.sin_port = htons(initialConnect.userPort);
					server.sin_addr.s_addr = inet_addr(serverIP);

					if(initialConnect.socketType == SOCK_STREAM)
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

						char block = malloc(initialConnect.blockSize);
					
						for(i =  0; i < initialConnect.numBlocks; i++)
						{

							memset(block, '\0',initialConnect.blockSize);

							strcpy(block,i);
           
								//sprintf(buffer, "%d of %s", i, initialConnect.numBlocks);
								//printf("%s\n", buffer );
								retval = send(conn_socket, block, sizeof(block), 0);
								if (retval < 0) {
									printf("send() failed: error %d\n", i);
#ifdef _WIN32					
									WSACleanup();
#endif					
									break;
								//memset(buffer, 0, totalSize);
							}
						}
						free(block);
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

