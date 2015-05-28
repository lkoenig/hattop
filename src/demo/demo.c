#include <stdio.h>
#include "hattop.h"

int main()
{
    hattop_t *srv;

    printf("hattop demo\n");

    srv = hattop_create();
    if(!srv) {
        fprintf(stderr, "Could not create server instance!\n");
        return 1;
    }

    hattop_listen(srv, 8080);
    hattop_accept_connection(srv);

    /* Quit after serving one request (lazy) */
    hattop_destroy(srv);
    return 0;
}

