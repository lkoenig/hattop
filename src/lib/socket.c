#include <stdio.h>
#include <string.h>
#include "socket.h"
#include "assert.h"

#ifdef _WIN32
#include "winsock2.h"

int platform_bootstrap(){
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        perror("WSAStartup failed");
        return -1;
    }
    return 0;
}

int close(socket_t s){
    return closesocket(s);
};

#else
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>

int platform_bootstrap(){
    return 0;
}

#endif //_WIN32

socket_t SOCKET_create(short portno)
{
    socket_t s;
    struct sockaddr_in serv_addr;

    /* Platform init. */
    if ((s = platform_bootstrap()) < 0) {
        return -1;
    }

    /* Create socket */
    if((s = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Could not create socket");
        return -1;
    }

    /* Set up server address */
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    /* Bind socket to address and port */
    if(bind(s, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Could not bind socket");
        return -1;
    }

    /* Start listening for connections */
    if(listen(s, 10) < 0) {
        perror("Could not listen for connections");
        return -1;
    }

    return s;
}

socket_t SOCKET_accept(socket_t s, int timeout_ms)
{
    fd_set readSet;
    struct timeval timeout;
    socket_t clientfd = -1;
    int clilen;
    struct sockaddr_in cli_addr;
    clilen = sizeof(cli_addr);

    FD_ZERO(&readSet);
    FD_SET(s, &readSet);

    timeout.tv_sec = timeout_ms / 1000;
    timeout.tv_usec = (timeout_ms % 1000) * 1000;

    /* check if incomming connection pending */
    if (select(s, &readSet, NULL, NULL, &timeout) == 1)
    {
        /* accept connection */
        clientfd = accept(s, (struct sockaddr *)&cli_addr, &clilen);
    }

    return clientfd;
}

int SOCKET_is_valid(socket_t s)
{
    return s >= 0;
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
    close(s);
}
