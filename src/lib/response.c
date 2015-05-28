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
