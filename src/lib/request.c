#include <stdio.h>
#include <string.h>
#include "request.h"
#include "response.h"

const char* hattop_asci_logo = "\
  ___   \n\
 |   |  \n\
_|___|_ \n\
 -o-o-  \n\
  www   \n\
.HaTToP \n";


void REQUEST_parse(socket_t s, const char *request)
{
#define REQUEST_LINE_SIZE 1024
    char request_line[REQUEST_LINE_SIZE];
    char *method, *uri, *version;
    int request_line_len;
    char *p;

    /* Verify that request contains with "\r\n\r\n" */
    if(!strstr(request, "\r\n\r\n")) {
        /* Bad request */
        RESPONSE_error(s, 400);
        return;
    }

    /* Find end of request-line */
    p = strstr(request, "\r\n");
    request_line_len = p - request;
    if(request_line_len > REQUEST_LINE_SIZE - 1) {
        /* Too large request URI */
        RESPONSE_error(s, 414);
    }

    /* Copy request-line to its own buffer */
    memcpy(request_line, request, request_line_len);
    request_line[request_line_len] = '\0';

    method = request_line;
    uri = NULL;
    version = NULL;

    /* Find delimiter between Method and Request-URI */
    p = strchr(request_line, ' ');
    if(!p) {
        RESPONSE_error(s, 400);
        return;
    }
    *p = '\0';
    uri = p+1;

    /* Find delimiter between Request-URI and HTTP-Version */
    p = strchr(uri, ' ');
    if(p) {
        *p = '\0';
        version = p+1;
    }

    fprintf(stderr, "\nMethod:       \"%s\"", method);
    fprintf(stderr, "\nRequest-URI:  \"%s\"", uri);
    fprintf(stderr, "\nHTTP-Version: \"%s\"", version);
    fprintf(stderr, "\n");

    /* Verify Method */
    if(strncmp(method, "GET", strlen(method))) {
        /* Only support GET for now */
        RESPONSE_error(s, 501);
        return;
    }

    /* Verify Request-URI */
    if(uri[0] != '/') {
        /* Request-URI should start with "/" */
        RESPONSE_error(s, 400);
        return;
    }
    /* Verify HTTP-Version */
    if(version) {
        if(strncmp(version, "HTTP/", 5)) {
            /* Version should start with "HTTP/" */
            RESPONSE_error(s, 400);
            return;
        }
    }

    RESPONSE_simple(s, "text/plain", hattop_asci_logo, strlen(hattop_asci_logo));
}
