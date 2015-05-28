#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include "hattop.h"

struct hattop_t
{
    int sockfd;
};

hattop_t *hattop_create()
{
    hattop_t *state = (hattop_t*)malloc(sizeof(hattop_t));
    if(state) {
        state->sockfd = -1;
    }
    return state;
}

void hattop_destroy(hattop_t *state)
{
    free((void*)state);
}

int hattop_listen(hattop_t *state, short portno)
{
    struct sockaddr_in serv_addr;

    /* Create socket */
    if(state->sockfd < 0) {
        if((state->sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            perror("Could not create socket");
            return -1;
        }
    }

    /* Set up server address */
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    /* Bind socket to address and port */
    if(bind(state->sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Could not bind socket");
        return -2;
    }

    /* Start listening for connections */
    if(listen(state->sockfd, 10) < 0) {
        perror("Could not listen for connections");
        return -3;
    }

    return 0;
}

void hattop_accept_connection(hattop_t *state)
{
    int clientfd, clilen;
    struct sockaddr_in cli_addr;
    clilen = sizeof(cli_addr);

    /* Accept client connection */
    clientfd = accept(state->sockfd, (struct sockaddr *)&cli_addr, &clilen);

    /* Dummy handler for testing */
    {
        char buf[] = "HTTP/1.1 200\r\n\r\nHello, world!\n";
        send(clientfd, buf, strlen(buf), 0);
        close(clientfd);
    }
}
