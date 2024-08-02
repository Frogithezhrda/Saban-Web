#ifndef HANDLESOCKET
#define HANDLESOCKET

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#define MAX_BUFFER 2048
#define PORT "90"
#define SERVER_ADDRESS "localhost"
#ifdef __cplusplus
extern "C" {
#endif


char* sendMessage(char* sendingBuffer);
#ifdef __cplusplus
}
#endif


#endif // HANDLESOCKET
