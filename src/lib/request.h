#ifndef REQUEST_H
#define REQUEST_H

#include "hattop.h"
#include "socket.h"

void REQUEST_parse(hattop_t *state, socket_t s, const char *request);

#endif
