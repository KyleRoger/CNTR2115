


#include "../inc/client.h"


int runClient(int argc, char* argv[])
{
unsigned short port = DEFAULT_PORT;
	int sockType = 0;
	int userPort = 0;
	int blockSize = 0;
	int numBlocks = 0;
	int initialMessage[4];
	int retval = 0;
	char * buffer = NULL;
	int totalSize = 0;
	int amountOfPackage = 0;
	unsigned int addr;
	struct sockaddr_in server;

#ifdef _WIN32
	WSADATA wsaData;
	SOCKET  conn_socket;
#endif
	if (argc != 4)
	{
		printf("Not correct amount of Agruments");
	}
	else
	{
		//Send sck type, port, block size, block number.
		if(strcmp(argv[0],"-TCP") == NULL)
		{
			sockType = SOCK_STREAM;
		}
		else if(strcmp(argv[0],"-UDP") == NULL)
		{
			sockType = SOCK_DGRAM;
		}
		else
		{
			printf("Invalid Argument.");
		}

		userPort = argv[1];
		blockSize = argv[2];
		numBlocks = argv[3];

		initialMessage[0] = sockType;
		initialMessage[1] = userPort;
		initialMessage[2] = blockSize;
		initialMessage[3] = numBlocks;

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
			server.sin_addr.s_addr = inet_addr(argv[1]);

			conn_socket = socket(AF_INET, SOCK_STREAM, 0); 
			if (conn_socket < 0) {
				printf("Client: Error Opening socket: Error\n");/
#ifdef _WIN32				
				WSACleanup();
#endif
			}

			else
			{
				if (connect(conn_socket, (struct sockaddr*)&server, sizeof(server)) < 0) {
					printf("connect() failed: \n");
#ifdef _WIN32									
					WSACleanup();
#endif			
				}

					sprintf(buffer, "%d", i);
						retval = send(conn_socket, initialMessage, totalSize, 0);





				totalSize = atoi(argv[2]);

				amountOfPackage = atoi(argv[3]);
				if ((amountOfPackage == 0) || (amountOfPackage > 1000000))
				{
					printf("Number is invalid number\n");
				}
				else
				{
					int i = 0;
					//loop base on the thrid agrument
					for (; i < amountOfPackage; i++)
					{
						sprintf(buffer, "%d", i);
						retval = send(conn_socket, buffer, totalSize, 0);
						//check to see if it was sent sucessfully
						if (retval < 0) {
							printf("send() failed: error %d\n", i);
#ifdef _WIN32					
							WSACleanup();
#endif					
							break;

						}
						else
						{
							printf("%i\n", i);
						}
						memset(buffer, 0, totalSize);
					}
#ifdef _WIN32
					closesocket(conn_socket);
					WSACleanup();
#endif
				}
			}
		}
	}

	return 0;
}

