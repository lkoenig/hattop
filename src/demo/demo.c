#include <stdio.h>
#include <string.h>
#include "hattop.h"
#include "uri.h"

void handler(void *connection, const char *uri)
{
    char buffer[2048];
    int buffer_pos = 0;
    const char* hattop_asci_logo =
    "  ___   \n"
    " |   |  \n"
    "_|___|_ \n"
    " -o-o-  \n"
    "  www   \n"
    ".HaTToP \n";

    buffer_pos += sprintf(buffer + buffer_pos, "<html><body>Document: %s<br/><br/><hr/><pre>%s", uri, hattop_asci_logo);

    hattop_uri * uri_struct = hattop_uri_create(uri);

    if(uri_struct){
        buffer_pos += sprintf(buffer + buffer_pos, "\n\nPath: %s", uri_struct->path);
        buffer_pos += sprintf(buffer + buffer_pos, "\nQuery parameters: %i", uri_struct->query_parameters.num);
        if(uri_struct->query_parameters.num){
            int i = 0;
            for(i=0; i<uri_struct->query_parameters.num; i++){
                buffer_pos += sprintf(buffer + buffer_pos, "\n\t%s:%s", uri_struct->query_parameters.keys[i], uri_struct->query_parameters.values[i]);
            }
        }
    }
    else{
        buffer_pos += sprintf(buffer + buffer_pos, "\n\nInvalid Uri");
    }

    buffer_pos += sprintf(buffer + buffer_pos, "</pre></body></html>");
    hattop_response_simple(connection, "text/html", buffer, strlen(buffer));

    hattop_uri_destroy(uri_struct);
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

    hattop_start_serving(srv, 8081);
    printf("Press RETURN to quit\n");
    getchar();
    printf("Exiting");
    hattop_stop_serving(srv);

    hattop_destroy(srv);
    return 0;
}

