#include "handleSocket.h"

#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable:4996)

char* sendMessage(char* sendingBuffer)
{
    WSADATA wsaData;
    SOCKET connectSocket = INVALID_SOCKET;
    struct addrinfo* result = NULL;
    struct addrinfo hints;
    char* recvbuf = NULL;
    int iResult = 0;

    recvbuf = (char*)malloc(sizeof(char) * MAX_BUFFER);
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) 
    {
        printf("WSAStartup failed: %d\n", iResult);
        return;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    iResult = getaddrinfo(SERVER_ADDRESS, PORT, &hints, &result);
    if (iResult != 0) 
    {
        printf("getaddrinfo failed: %d\n", iResult);
        WSACleanup();
        return;
    }

    // Attempt to connect to the first address returned by getaddrinfo
    connectSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (connectSocket == INVALID_SOCKET) 
    {
        printf("Error at socket(): %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return;
    }

    // Connect to server.
    iResult = connect(connectSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) 
    {
        closesocket(connectSocket);
        connectSocket = INVALID_SOCKET;
    }

    freeaddrinfo(result);

    if (connectSocket == INVALID_SOCKET) 
    {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return;
    }

    // Send an initial buffer
    iResult = send(connectSocket, sendingBuffer, (int)strlen(sendingBuffer), 0);
    if (iResult == SOCKET_ERROR) 
    {
        printf("send failed: %d\n", WSAGetLastError());
        closesocket(connectSocket);
        WSACleanup();
        return;
    }

    printf("Bytes Sent: %ld\n", iResult);

    // shutdown the connection since no more data will be sent
    iResult = shutdown(connectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) 
    {
        printf("shutdown failed: %d\n", WSAGetLastError());
        closesocket(connectSocket);
        WSACleanup();
        return;
    }

    // Receive until the peer closes the connection
    iResult = recv(connectSocket, recvbuf, MAX_BUFFER, 0);
    recvbuf = realloc(recvbuf, (sizeof(char) * iResult) + 1);
    recvbuf[iResult] = '\0';
    if (iResult > 0)
    {
        printf("Bytes received: %d - %s\n", iResult, recvbuf);
    }
    // cleanup
    closesocket(connectSocket);
    WSACleanup();
    return recvbuf;
}
