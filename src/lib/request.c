#include <stdio.h>
#include <string.h>
#include "request.h"

void REQUEST_parse(hattop_t *state, socket_t s, const char *request)
{
#define REQUEST_LINE_SIZE 1024
    char request_line[REQUEST_LINE_SIZE];
    char *method, *uri, *version;
    int request_line_len;
    char *p;

    /* Verify that request contains with "\r\n\r\n" */
    if(!strstr(request, "\r\n\r\n")) {
        /* Bad request */
        hattop_response_error((void*)&s, 400);
        return;
    }

    /* Find end of request-line */
    p = strstr(request, "\r\n");
    request_line_len = p - request;
    if(request_line_len > REQUEST_LINE_SIZE - 1) {
        /* Too large request URI */
        hattop_response_error((void*)&s, 414);
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
        hattop_response_error((void*)&s, 400);
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

    /* Verify Method */
    if(strncmp(method, "GET", strlen(method))) {
        /* Only support GET for now */
        hattop_response_error((void*)&s, 501);
        return;
    }

    /* Verify Request-URI */
    if(uri[0] != '/') {
        /* Request-URI should start with "/" */
        hattop_response_error((void*)&s, 400);
        return;
    }
    /* Verify HTTP-Version */
    if(version) {
        if(strncmp(version, "HTTP/", 5)) {
            /* Version should start with "HTTP/" */
            hattop_response_error((void*)&s, 400);
            return;
        }
    }

    if(state->handler) {
        state->handler((void*)&s, uri);
    } else {
        hattop_response_error((void*)&s, 500);
    }
}
