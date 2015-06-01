#include <stdio.h>
#include <string.h>
#include "socket.h"
#include "assert.h"

#ifdef _WIN32
#include "winsock2.h"

#else /* Linux, etc */
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#define INVALID_SOCKET -1

#endif //_WIN32

socket_t SOCKET_create(short portno)
{
    socket_t s;
    struct sockaddr_in serv_addr;

#ifdef _WIN32
    static int first_run = 1;
    if(first_run) {
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            perror("WSAStartup failed");
            return INVALID_SOCKET;
        }
        first_run = 0;
    }
#endif

    /* Create socket */
    if((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        perror("Could not create socket");
        return INVALID_SOCKET;
    }

    /* Set up server address */
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    /* Bind socket to address and port */
    if(bind(s, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Could not bind socket");
        return INVALID_SOCKET;
    }

    /* Start listening for connections */
    if(listen(s, 10) < 0) {
        perror("Could not listen for connections");
        return INVALID_SOCKET;
    }

    return s;
}

socket_t SOCKET_accept(socket_t s, int timeout_ms)
{
    struct timeval timeout;
    int clilen;
    struct sockaddr_in cli_addr;

    clilen = sizeof(cli_addr);
    timeout.tv_sec = timeout_ms / 1000;
    timeout.tv_usec = (timeout_ms % 1000) * 1000;

    fd_set readSet;
    FD_ZERO(&readSet);
    FD_SET(s, &readSet);

    /* check if incomming connection pending */
    if (select(s+1, &readSet, NULL, NULL, &timeout) == 1)
    {
        /* accept connection */
        return accept(s, (struct sockaddr *)&cli_addr, &clilen);
    }

    return INVALID_SOCKET;
}

int SOCKET_is_valid(socket_t s)
{
    return s != INVALID_SOCKET;
}

void SOCKET_send(socket_t s, const char *buf, int buflen)
{
    send(s, buf, buflen, 0);
}

int SOCKET_recv(socket_t s, char *buf, int bufsize)
{
    return (int)recv(s, (void*)buf, (size_t)bufsize, 0);
}

void SOCKET_close(socket_t s)
{
#ifdef _WIN32
    closesocket(s);
#else
    close(s);
#endif
}
