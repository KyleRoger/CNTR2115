


#include "../inc/client.h"

struct Message {
	int sockType;
	int userPort;
	int blockSize;
	int numBlocks;

};

int runClient(int argc, char* argv[])
{
	struct Message initialConnect;
	int port = 15000;
	int i =0;
	char* serverIP = NULL;
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
				initialConnect.sockType = SOCK_STREAM;
			}
			else if(strcmp(argv[i],"-UDP") == 0)
			{
				initialConnect.sockType = SOCK_DGRAM;
			}
			else if(strcmp(argv[i], "-a") ==0)
			{
				serverIP = argv[i + 1];
			}
			else if(strcmp(argv[i],"-p") == 0)
			{
				initialConnect.userPort = atoi(argv[i+1]);
			}
			else if(strcmp(argv[i],"-s") == 0)
			{
				initialConnect.blockSize = atoi(argv[i+1]);
			}
			else if(strcmp(argv[i],"-n") == 0)
			{
				initialConnect.numBlocks = atoi(argv[i+1]);
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

				printf("%i\n", initialConnect.blockSize);
				retval = write(conn_socket, initialConnect, sizeof(initialConnect));





				/*totalSize = atoi(argv[2]);

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
				}*/
			}
		}
	}

	return 0;
}

