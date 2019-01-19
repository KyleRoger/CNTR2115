#include "../inc/client.h"


#define SERVER "127.0.0.1"	//ip address of udp server
#define BUFLEN 512	//Max length of buffer
#define PORT 15009	//The port on which to listen for incoming data
#define serverIP "192.168.223.128"
int main(int argc, char* argv[])
{
/*struct sockaddr_in si_other;
	int s, slen=sizeof(si_other);
	char buf[BUFLEN];
	char message[BUFLEN];
	WSADATA wsa;

	//Initialise winsock
	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
	{
		printf("Failed. Error Code : %d",WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	printf("Initialised.\n");
	
	//create socket
	if ( (s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR)
	{
		printf("socket() failed with error code : %d" , WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	
	//setup address structure
	memset((char *) &si_other, 0, sizeof(si_other));
	si_other.sin_family = AF_INET;
	si_other.sin_port = htons(PORT);
	si_other.sin_addr.S_un.S_addr = inet_addr(SERVER);
	
	//start communication
	while(1)
	{
		printf("Enter message : ");
		gets(message);
		
		//send the message
		if (sendto(s, message, strlen(message) , 0 , (struct sockaddr *) &si_other, slen) == SOCKET_ERROR)
		{
			printf("sendto() failed with error code : %d" , WSAGetLastError());
			exit(EXIT_FAILURE);
		}
		
		//receive a reply and print it
		//clear the buffer by filling null, it might have previously received data
		memset(buf,'\0', BUFLEN);
		//try to receive some data, this is a blocking call
		if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen) == SOCKET_ERROR)
		{
			printf("recvfrom() failed with error code : %d" , WSAGetLastError());
			exit(EXIT_FAILURE);
		}
		
		puts(buf);
	}

	closesocket(s);
	WSACleanup();

	return 0;*/
	return runClient(argc, argv);

socketInfo initialConnect;
	int port = DEFAULT_PORT;
	int i =0;
	int socketType = 0;
	int userPort = 13008;
	int numBlocks = 0;
	int blockSize = 0;
	char* block = NULL;
	// char* serverIP = "192.168.223.128";
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
	SOCKET  message_socket;
#endif
#ifdef linux
	int message_socket = 0;
#endif

Sleep(2000);
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

					
						message_socket = socket(AF_INET, SOCK_STREAM, 0);
					
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
           					printf("block size is: %zu\n", sizeof(block));
           					/*int len = sizeof(server);
           					if (sendto(message_socket, block, strlen(block) , 0 , (struct sockaddr *) &server, len) == SOCKET_ERROR)
							{
								printf("sendto() failed with error code : %d" , WSAGetLastError());
								exit(EXIT_FAILURE);
							}
*/							retval = send(message_socket, block, blockSize, 0); //sendto (message_socket, block, strlen(block), 0, (struct sockaddr *)&server, len);
							if (retval < 0) 
							{
								printf("\nsend() failed with error code : %d\n" , WSAGetLastError());
								printf("send() failed: error %d\n", i);
#ifdef _WIN32					
								WSACleanup();
#endif					
								break;
							}
						}

						
#ifdef _WIN32
						closesocket(message_socket);
						WSACleanup();					
#endif
#ifdef linux
						close(message_socket);
#endif
					}
				}
		

	return 0;
}