#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "socket.h"

socket_t SOCKET_create(short portno)
{
    socket_t s;
    struct sockaddr_in serv_addr;

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
    socket_t clientfd;
    int clilen;
    struct sockaddr_in cli_addr;
    clilen = sizeof(cli_addr);

    /* Accept client connection */
    clientfd = accept(s, (struct sockaddr *)&cli_addr, &clilen);

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

void SOCKET_close(socket_t s)
{
    close(s);
}
