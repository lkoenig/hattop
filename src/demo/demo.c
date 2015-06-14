#include <stdio.h>
#include <string.h>
#include "hattop.h"

struct demo_state{
    int requests;
};

void demo_handle(void *state, void *connection, const char *uri)
{
    char buffer[2048];
    const char* hattop_asci_logo =
    "  ___   \n"
    " |   |  \n"
    "_|___|_ \n"
    " -o-o-  \n"
    "  www   \n"
    ".HaTToP \n";

    struct demo_state * h_state = (struct demo_state *)state;
    h_state->requests++;

    sprintf(buffer, "<html><body>Document: %s<br/><br/><hr/><pre>%s \nrequests: %i</pre></body></html>", uri, hattop_asci_logo, h_state->requests);

    hattop_response_simple(connection, "text/html", buffer, strlen(buffer));
}

int main()
{
    hattop_t *srv;
    handler_t handler;

    struct demo_state handler_state;
    handler_state.requests = 0;
    handler.handle = demo_handle;
    handler.state = &handler_state;

    printf("hattop demo\n");

    srv = hattop_create();
    if(!srv) {
        fprintf(stderr, "Could not create server instance!\n");
        return 1;
    }

    hattop_register_handler(srv, &handler);

    hattop_start_serving(srv, 8080);
    printf("Press RETURN to quit\n");
    getchar();
    printf("Exiting");
    hattop_stop_serving(srv);

    hattop_destroy(srv);
    return 0;
}

