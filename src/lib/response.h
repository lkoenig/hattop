#ifndef RESPONSE_H
#define RESPONSE_H

#include "socket.h"

void RESPONSE_simple(socket_t s, const char *content_type, const char *body, int body_len);
void RESPONSE_error(socket_t s, int status);

#endif
