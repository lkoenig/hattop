#include <stdio.h>
#include <string.h>
#include "hattop.h"

void handler(void *connection, const char *uri)
{
    char buffer[2048];
    const char* hattop_asci_logo =
    "  ___   \n"
    " |   |  \n"
    "_|___|_ \n"
    " -o-o-  \n"
    "  www   \n"
    ".HaTToP \n";

    sprintf(buffer, "<html><body>Document: %s<br/><br/><hr/><pre>%s</pre></body></html>", uri, hattop_asci_logo);

    hattop_response_simple(connection, "text/html", buffer, strlen(buffer));
}

int main()
{
    hattop_t *srv;

    printf("hattop demo\n");

    srv = hattop_create();
    if(!srv) {
        fprintf(stderr, "Could not create server instance!\n");
        return 1;
    }

    hattop_register_handler(srv, handler);

    hattop_start_serving(srv, 8080);
    printf("Press RETURN to quit\n");
    getchar();
    printf("Exiting");
    hattop_stop_serving(srv);

    hattop_destroy(srv);
    return 0;
}

