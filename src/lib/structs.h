#ifndef DEFINES_H
#define DEFINES_H

#include "hattop.h"
#include "thread.h"

/* Webserver state */
typedef struct hattop_t
{
    thread_t server_thread;
    short portno;
    int continue_serving;
    handler_t *handler;
} hattop_t;

#endif
