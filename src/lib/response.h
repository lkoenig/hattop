#ifndef RESPONSE_H
#define RESPONSE_H

#include "socket.h"

void RESPONSE_simple(socket_t s, const char *content_type, const char *body, int body_len);

#endif
