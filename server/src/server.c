/*
* -----------------------------------------------------------------------
* File			: makefile
* Project		: ispeed
* Author 		: Arie Kraayenbrink
* Editors		: Arie kraayenbrink
* Date 			: Jan. 8, 2019
* Description 	: This makefile will compile and link all source files
*             	: in the server part of the system.  
*				: 
*				: Steps for creating windows sockets on server.
*				   1. Initialize Winsock.
*				   2. Create a socket.
*				   3. Bind the socket.
*				   4. Listen on the socket for a client.
*				   5. Accept a connection from a client.
*				   6. Receive and send data.
*				   7. Disconnect.
*
* Credit		: https://docs.microsoft.com/en-us/windows/desktop/winsock/getting-started-with-winsock
* ------------------------------------------------------------------------
*/

#include "../inc/server.h"

#undef UNICODE

int runServer()
{
	int success = FAILURE;

	#ifdef _WIN32

	WSADATA wsaData;

	int iResult;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != 0) 
	{
	    printf("WSAStartup failed: %d\n", iResult);
	    return 1;
	}

	struct addrinfo *result = NULL, *ptr = NULL, hints;

	ZeroMemory(&hints, sizeof (hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the local address and port to be used by the server
	iResult = getaddrinfo(NULL, PORT, &hints, &result);
	if (iResult != 0) {
	    printf("getaddrinfo failed: %d\n", iResult);
	    WSACleanup();
	    return 1;
	}

	SOCKET ListenSocket = INVALID_SOCKET;

	// Create a SOCKET for the server to listen for client connections
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	if (ListenSocket == INVALID_SOCKET) {
	    printf("Error at socket(): %ld\n", WSAGetLastError());
	    freeaddrinfo(result);
	    WSACleanup();
	    return 1;
	}

	// Setup the TCP listening socket
    iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(result);

    //listen
    if ( listen( ListenSocket, SOMAXCONN ) == SOCKET_ERROR ) {
	    printf( "Listen failed with error: %ld\n", WSAGetLastError() );
	    closesocket(ListenSocket);
	    WSACleanup();
	    return 1;
	}

	SOCKET ClientSocket;

	ClientSocket = INVALID_SOCKET;

	// Accept a client socket
	ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET) {
	    printf("accept failed: %d\n", WSAGetLastError());
	    closesocket(ListenSocket);
	    WSACleanup();
	    return 1;
	}

	//Send and Recieve
	#define DEFAULT_BUFLEN 512

	char recvbuf[DEFAULT_BUFLEN];
	int iResult, iSendResult;
	int recvbuflen = DEFAULT_BUFLEN;

	// Receive until the peer shuts down the connection
	do {

	    iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
	    if (iResult > 0) {
	        printf("Bytes received: %d\n", iResult);

	        // Echo the buffer back to the sender
	        iSendResult = send(ClientSocket, recvbuf, iResult, 0);
	        if (iSendResult == SOCKET_ERROR) {
	            printf("send failed: %d\n", WSAGetLastError());
	            closesocket(ClientSocket);
	            WSACleanup();
	            return 1;
	        }
	        printf("Bytes sent: %d\n", iSendResult);
	    } else if (iResult == 0)
	        printf("Connection closing...\n");
	    else {
	        printf("recv failed: %d\n", WSAGetLastError());
	        closesocket(ClientSocket);
	        WSACleanup();
	        return 1;
	    }

	} while (iResult > 0);

	//Disconnect
	// shutdown the send half of the connection since no more data will be sent
	iResult = shutdown(ClientSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
	    printf("shutdown failed: %d\n", WSAGetLastError());
	    closesocket(ClientSocket);
	    WSACleanup();
	    return 1;
	}

	// cleanup
	closesocket(ClientSocket);
	WSACleanup();

	#endif

	return success;
}

int windowsSockets(void)
{

    WSADATA wsaData;
    int iResult;

    SOCKET ListenSocket = INVALID_SOCKET;
    SOCKET ClientSocket = INVALID_SOCKET;

    struct addrinfo *result = NULL;
    struct addrinfo hints;

    int iSendResult;
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;
    
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if ( iResult != 0 ) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Create a SOCKET for connecting to server
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    // Setup the TCP listening socket
    iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(result);

    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    // Accept a client socket
    ClientSocket = accept(ListenSocket, NULL, NULL);
    if (ClientSocket == INVALID_SOCKET) {
        printf("accept failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    // No longer need server socket
    closesocket(ListenSocket);

    // Receive until the peer shuts down the connection
    do {

        iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0) {
            printf("Bytes received: %d\n", iResult);

        // Echo the buffer back to the sender
            iSendResult = send( ClientSocket, recvbuf, iResult, 0 );
            if (iSendResult == SOCKET_ERROR) {
                printf("send failed with error: %d\n", WSAGetLastError());
                closesocket(ClientSocket);
                WSACleanup();
                return 1;
            }
            printf("Bytes sent: %d\n", iSendResult);
        }
        else if (iResult == 0)
            printf("Connection closing...\n");
        else  {
            printf("recv failed with error: %d\n", WSAGetLastError());
            closesocket(ClientSocket);
            WSACleanup();
            return 1;
        }

    } while (iResult > 0);

    // shutdown the connection since we're done
    iResult = shutdown(ClientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ClientSocket);
        WSACleanup();
        return 1;
    }

    // cleanup
    closesocket(ClientSocket);
    WSACleanup();

    return 0;
}
