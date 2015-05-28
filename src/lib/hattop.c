#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hattop.h"
#include "socket.h"

const char* hattop_asci_logo = "\
  ___   \n\
 |   |  \n\
_|___|_ \n\
 -o-o-  \n\
  www   \n\
.HaTToP \n";

struct hattop_t
{
};

hattop_t *hattop_create()
{
    hattop_t *state = (hattop_t*)malloc(sizeof(hattop_t));
    if(state) {
        /* Initialize state */
    }
    return state;
}

void hattop_destroy(hattop_t *state)
{
    free((void*)state);
}

int hattop_listen(hattop_t *state, short portno)
{
    socket_t s;
    s = SOCKET_create(portno);

    while(1) {
        socket_t s_client = SOCKET_accept(s, 1000);
        if(SOCKET_is_valid(s_client)) {
            /* Dummy handler for testing */
            char head[] = "HTTP/1.1 200\r\n\r\n";
            SOCKET_send(s_client, head, strlen(head));
            SOCKET_send(s_client, hattop_asci_logo, strlen(hattop_asci_logo));
            SOCKET_close(s_client);
        }
    }

    return 0;
}
