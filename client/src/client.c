


#include "../inc/client.h"


int runClient()
{

#ifdef _WIN32
	struct addrinfo *result = NULL,
	                *ptr = NULL,
	                hints;

	ZeroMemory( &hints, sizeof(hints) );
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
#endif
	

	char byte1000[1000] = "";
	char byte2000[2000] = "";
	char byte5000[5000] = "";
	char byte10000[10000] = "";
	if (argc != 4)
	{
		printf("Incorrect Number of Arguments.\n");
	}
	else
	{
		//Windows Windows Sockets implementation
		int iResult;

		// Initialize Winsock
		iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
		if (iResult != 0)
		{
	   		printf("WSAStartup failed: %d\n", iResult);
	    	return 1;
    	}
		else
		{
			// Resolve the server address and port
		iResult = getaddrinfo(argv[1], DEFAULT_PORT, &hints, &result);
		if (iResult != 0) 
		{
		    printf("getaddrinfo failed: %d\n", iResult);
		    WSACleanup();
		    return 1;
		}

		else
		{
			if (connect(conn_socket, (struct sockaddr*)&server, sizeof(server)) < 0) 
			{
				printf("connect() failed: \n");									
				WSACleanup();		
				
			}
		}

		SOCKET ConnectSocket = INVALID_SOCKET;

		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, 
    	ptr->ai_protocol);

    	if (ConnectSocket == INVALID_SOCKET) 
    	{
		    printf("Error at socket(): %ld\n", WSAGetLastError());
		    freeaddrinfo(result);
		    WSACleanup();
		    return 1;
		}

	}
}