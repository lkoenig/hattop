#include <stdio.h>
#include <string.h>
#include "response.h"

void RESPONSE_simple(socket_t s, const char *content_type, const char *body, int body_len)
{
    char header[1024];
    int status = 200;
    sprintf(header, "HTTP/1.1 %d\r\nContent-Type: %s\r\nContent-Length: %d\r\n\r\n", status, content_type, body_len);
    SOCKET_send(s, header, strlen(header));
    SOCKET_send(s, body, body_len);
}

void RESPONSE_error(socket_t s, int status)
{
    char resp[1024];
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
