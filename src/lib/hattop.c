#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hattop.h"
#include "structs.h"
#include "socket.h"
#include "request.h"

#define REQUEST_BUF_SIZE 8192

hattop_t *hattop_create()
{
    hattop_t *state = (hattop_t*)malloc(sizeof(hattop_t));
    if(state) {
        state->server_thread = 0;
        state->continue_serving = 0;
        state->handler = NULL;
    }
    return state;
}

void hattop_destroy(hattop_t *state)
{
    free((void*)state);
}

void hattop_register_handler(hattop_t *state, handler_t handler)
{
    state->handler = handler;
}

static void *hattop_server_thread(void *arg)
{
    socket_t s;
    hattop_t *state = (hattop_t*)arg;
    s = SOCKET_create(state->portno);
    if(!SOCKET_is_valid(s)) {
        return NULL;
    }

    while(state->continue_serving) {
        socket_t s_client = SOCKET_accept(s, 100);
        if(SOCKET_is_valid(s_client)) {
            /* Dummy handler for testing */
            char recv_buf[REQUEST_BUF_SIZE];
            int bytes_read = 0;

            /* Read request */
            do {
                int ret = SOCKET_recv(s_client, &recv_buf[bytes_read], REQUEST_BUF_SIZE - bytes_read - 1);
                if(ret > 0) {
                    bytes_read += ret;
                    recv_buf[bytes_read] = '\0';

                    /* Find the end of the request */
                    if(strstr(recv_buf, "\r\n\r\n")) {
                        break;
                    }
                }
            } while(bytes_read < REQUEST_BUF_SIZE - 1);

            REQUEST_parse(state, s_client, recv_buf);
            SOCKET_close(s_client);
        }
    }
}

int hattop_start_serving(hattop_t *state, short portno)
{
    state->portno = portno;
    state->continue_serving = 1;
    THREAD_create(&state->server_thread, hattop_server_thread, state);

    return 0;
}

void hattop_stop_serving(hattop_t *state)
{
    if(state->server_thread) {
        state->continue_serving = 0;
        THREAD_join(state->server_thread);
        state->server_thread = 0;
    }
}

void hattop_response_simple(void *connection, const char *content_type, const char *body, int body_len)
{
    char header[1024];
    int status = 200;
    socket_t s = *(socket_t*)connection;
    sprintf(header, "HTTP/1.1 %d\r\nContent-Type: %s\r\nContent-Length: %d\r\n\r\n", status, content_type, body_len);
    SOCKET_send(s, header, strlen(header));
    SOCKET_send(s, body, body_len);
}

void hattop_response_error(void *connection, int status)
{
    char resp[1024];
    socket_t s = *(socket_t*)connection;
    const char *reason_phrase = "Error";
    switch(status) {
        case 101: reason_phrase = "Switching Protocols"; break;

        case 200: reason_phrase = "OK"; break;
        case 201: reason_phrase = "Created"; break;
        case 202: reason_phrase = "Accepted"; break;
        case 203: reason_phrase = "Non-Authoritative Information"; break;
        case 204: reason_phrase = "No Content"; break;
        case 205: reason_phrase = "Reset Content"; break;
        case 206: reason_phrase = "Partial Content"; break;

        case 300: reason_phrase = "Multiple Choices"; break;
        case 301: reason_phrase = "Moved Permanently"; break;
        case 302: reason_phrase = "Found"; break;
        case 303: reason_phrase = "See Other"; break;
        case 304: reason_phrase = "Not Modified"; break;
        case 305: reason_phrase = "Use Proxy"; break;
        case 307: reason_phrase = "Temporary Redirect"; break;

        case 400: reason_phrase = "Bad Request"; break;
        case 401: reason_phrase = "Unauthorized"; break;
        case 402: reason_phrase = "Payment Required"; break;
        case 403: reason_phrase = "Forbidden"; break;
        case 404: reason_phrase = "Not Found"; break;
        case 405: reason_phrase = "Method Not Allowed"; break;
        case 406: reason_phrase = "Not Acceptable"; break;
        case 407: reason_phrase = "Proxy Authentication Required"; break;
        case 408: reason_phrase = "Request Time-out"; break;
        case 409: reason_phrase = "Conflict"; break;
        case 410: reason_phrase = "Gone"; break;
        case 411: reason_phrase = "Length Required"; break;
        case 412: reason_phrase = "Precondition Failed"; break;
        case 413: reason_phrase = "Request Entity Too Large"; break;
        case 414: reason_phrase = "Request-URI Too Large"; break;
        case 415: reason_phrase = "Unsupported Media Type"; break;
        case 416: reason_phrase = "Requested range not satisfiable"; break;
        case 417: reason_phrase = "Expectation Failed"; break;

        case 500: reason_phrase = "Internal Server Error"; break;
        case 501: reason_phrase = "Not Implemented"; break;
        case 502: reason_phrase = "Bad Gateway"; break;
        case 503: reason_phrase = "Service Unavailable"; break;
        case 504: reason_phrase = "Gateway Time-out"; break;
        case 505: reason_phrase = "HTTP Version not supported"; break;
    }

    sprintf(resp, "HTTP/%s %d %s\r\nContent-Type: text/html\r\n\r\n<html><head><title>%d %s</title></head><body><h1>%d %s</h1></body></html>", "1.1", status, reason_phrase, status, reason_phrase, status, reason_phrase);
    SOCKET_send(s, resp, strlen(resp));
}
