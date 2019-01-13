


#include "../inc/client.h"


int runClient(int argc, char* argv[])
{
unsigned short port = DEFAULT_PORT;
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
	
	char buffer1000[1000] = "";
	char buffer2000[2000] = "";
	char buffer5000[5000] = "";
	char buffer10000[10000] = "";
	if (argc != 4)
	{
		printf("Not correct amount of Agruments");
	}
	else
	{
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
				printf("Client: Error Opening socket: Error\n");
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

				totalSize = atoi(argv[2]);

				if (totalSize == 1000)
				{
					buffer = buffer1000;
				}
				else if (totalSize == 2000)
				{
					buffer = buffer2000;

				}
				else if (totalSize == 5000)
				{
					buffer = buffer5000;
				}
				else if (totalSize == 10000)
				{
					buffer = buffer1000;
				}

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

