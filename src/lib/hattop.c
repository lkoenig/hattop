#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hattop.h"
#include "socket.h"
#include "response.h"

#define REQUEST_BUF_SIZE 8192

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
    if(!SOCKET_is_valid(s)) {
        return -1;
    }

    while(1) {
        socket_t s_client = SOCKET_accept(s, 1000);
        if(SOCKET_is_valid(s_client)) {
            /* Dummy handler for testing */
            char recv_buf[REQUEST_BUF_SIZE];
            int bytes_read = 0;

            /* Read request */
            fprintf(stderr, "\n");
            do {
                int ret = SOCKET_recv(s_client, &recv_buf[bytes_read], REQUEST_BUF_SIZE - bytes_read - 1);
                if(ret > 0) {
                    bytes_read += ret;
                    recv_buf[bytes_read] = '\0';

                    /* Find the end of the request */
                    if(strstr(recv_buf, "\r\n\r\n")) {
                        fprintf(stderr, "%s", recv_buf);
                        break;
                    }
                }
            } while(bytes_read < REQUEST_BUF_SIZE - 1);

            RESPONSE_simple(s_client, "text/plain", hattop_asci_logo, strlen(hattop_asci_logo));
            SOCKET_close(s_client);
        }
        fprintf(stderr, ".");
    }

    return 0;
}
