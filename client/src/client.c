


#include "../inc/client.h"


int runClient(int argc, char* argv[])
{
	socketInfo initialConnect;
	int port = DEFAULT_PORT;
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
	SOCKET  initial_socket, message_socket;
#endif
#ifdef linux
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
				if (connect(initial_socket, (struct sockaddr*)&server, sizeof(server)) < 0) 
				{
					printf("connect() failed: %s \n", serverIP);
#ifdef _WIN32									
					WSACleanup();
#endif			
				}
				printf("buf is: %s\n", buf);
				retval = send(initial_socket, buf, sizeof(buf), 0);
				if (retval < 0) 
				{
					printf("\nsend() failed with error code : %d\n" , WSAGetLastError());
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

Sleep(1000);
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
							printf("\nConnect() failed with error code : %d\n" , WSAGetLastError());
							WSACleanup();
#endif			
						}
					
						for(i =  0; i < numBlocks; i++)
						{
           					sprintf(block, "%d", i);
           					printf("%s ", block);
           					printf("Block size: %zu\n", sizeof(block));
           					/*int len = sizeof(server);
           					if (sendto(message_socket, block, strlen(block) , 0 , (struct sockaddr *) &server, len) == SOCKET_ERROR)
							{
								printf("sendto() failed with error code : %d" , WSAGetLastError());
								exit(EXIT_FAILURE);
							}
*/							retval = send(message_socket, block, blockSize, 0); //sendto (message_socket, block, strlen(block), 0, (struct sockaddr *)&server, len);
							if (retval < 0) 
							{
								printf("send() failed: error.\n\tI = %d\n", i);
#ifdef _WIN32					
								printf("\nsend() failed with error code : %d\n" , WSAGetLastError());
								WSACleanup();
#endif					
								break;
							}
							memset(block, 0, blockSize);
						}

						#ifdef DEBUG
						printf("\n%s\n", "Finnished sending blocks, sending Bye");
						#endif

						strcpy(buf, "Bye");
						retval = send(initial_socket, buf, sizeof(buf), 0);
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


/*int GetConnection()
{

			server.sin_family = AF_INET;
			server.sin_port = htons(port);
			server.sin_addr.s_addr = inet_addr(serverIP);
}*/



/*


#define DEFAULT_PORT 5001
#define WIN32_LEAN_AND_MEAN
#ifdef _WIN32
	#include <winsock2.h>
	#include <Ws2tcpip.h>
	#pragma comment(lib, "Ws2_32.lib")
#else
	#include <string.h> 
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <netdb.h>
	#include <unistd.h>
#include <sys/time.h>
#endif


#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BILLION  1000000000L;



 
int main(int argc, char **argv) {

	char *interface = NULL;
	unsigned short port = DEFAULT_PORT;
	int size = 0;
	int numberToSend = 0;
	int retval;
	char* address = NULL;
	int fromlen =0;
	char *buffer = ""; 
	char buffer1000[1000] = "";
	char buffer2000[2000] = "";
	char buffer5000[5000] = "";
	char buffer10000[10000] = "";
	double diff = 0.0;
	int socket_type = SOCK_STREAM;
	struct sockaddr_in local, from;
	unsigned long micros = 0;
	double millis = 0.0;
#ifdef _WIN32
		//timer in Windows
	clock_t start, end;
#else
  struct timeval start, end;

#endif

#ifdef _WIN32
	WSADATA wsaData;
	SOCKET listen_socket, msgsock;
#else
	int listen_socket, msgsock =0;
    	double accum = 0.0;
#endif

	int loopContinue = 1;

	address = argv[1];
	size = atoi(argv[2]);
	numberToSend = atoi(argv[3]);
	if ((numberToSend == 0) || (numberToSend > 1000000))
	{
		printf("Number is invalid number\n");
	}
	else
	{

		//1000
		if (size == 1000)
		{
			buffer = buffer1000;
		}
		//2000
		else if (size == 2000)
		{
			buffer = buffer2000;
		}
		//5000
		else if (size == 5000)
		{
			buffer = buffer5000;
		}
		//10000
		else if (size == 10000)
		{
			buffer = buffer10000;
			size = 10000;
		}

		while (loopContinue)
		{
#ifdef _WIN32
			if ((retval = WSAStartup(0x202, &wsaData)) != 0) {
				printf("WSAStartup failed with error\n");
				WSACleanup();
				loopContinue = 0;
				break;
			}
#endif
			//local
			local.sin_family = AF_INET;
			local.sin_addr.s_addr = inet_addr(address);
			local.sin_port = htons(port);

			listen_socket = socket(AF_INET, SOCK_STREAM, 0);
			//check the listen_socket to see if it fails
			if (listen_socket < 0) {
				printf("socket() failed with error \n");
#ifdef _WIN32			
				WSACleanup();
#endif
				loopContinue = 0;
				break;
			}
			//bind with the socket
			if (bind(listen_socket, (struct sockaddr*)&local, sizeof(local)) < 0) {
				printf("bind() failed with error\n");
#ifdef _WIN32			
				WSACleanup();
#endif			
				loopContinue = 0;
				//break when it fails
				break;
			}
			if (socket_type != SOCK_DGRAM)
			{
				//check error
				if (listen(listen_socket, 5) < 0) {
					printf("listen() failed with error\n");
#ifdef _WIN32			
					WSACleanup();
#endif
					loopContinue = 0;
					break;
				}
			}
			break;

		}
		int package = 0;
		int bytesInOrder = 0;
		if (loopContinue != 0)
		{

			fromlen = sizeof(from);
			msgsock = accept(listen_socket, (struct sockaddr*)&from, &fromlen);
			if (msgsock < 0) {
				printf("accept() error \n");
#ifdef _WIN32			
				WSACleanup();
#endif				
			}
			int i = 0;
#ifdef _WIN32
			start = clock();
#else
			gettimeofday(&start, NULL);
#endif
			int convertToInt = 0;
			//loop 
			for (; i < numberToSend; i++)
			{
				//recv
				retval = recv(msgsock, buffer, size, MSG_WAITALL);
				//check if you recv something
				if (retval > 0)
				{
					package++;
				}
				//convert to int
				convertToInt = atoi(buffer);
				//compare the counter when the recv
				if (i == convertToInt)
				{
					//bytes in order
					bytesInOrder++;
				}
				//clean the buffer
				memset(buffer, 0, size);

				//	if (retval < 0) {
					//	printf("recv() failed: error\n");
#ifdef _WIN32			
				//closesocket(msgsock);
#else
				close(msgsock);
#endif
				//continue;
			//}
				if (retval == 0) {
					printf("Client closed connection\n");
#ifdef _WIN32			
					closesocket(msgsock);
#else
					close(msgsock);
#endif				
					continue;
				}
				continue;
			}
#ifdef _WIN32			
			end = clock();
#else
			gettimeofday(&end, NULL);
			millis = (double)((end.tv_sec * 1000000 + end.tv_usec)
				- (start.tv_sec * 1000000 + start.tv_usec)) / 1000000;
#endif


#ifdef _WIN32			
			closesocket(msgsock);
#else

			close(msgsock);
#endif
		}
#ifdef _WIN32			
		micros = end - start;
		millis = micros / 1000;
#endif
		printf("Number of Package in order %d\nPackage Recv%d\nTime:%f\n", bytesInOrder, package, millis);
	}
}
*/