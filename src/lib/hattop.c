#include <stdlib.h>
#include "hattop.h"

#define DEFAULT_HTTP_PORT 8080

struct hattop_t
{
    int sockfd;
    short portno;
};

hattop_t *hattop_create()
{
    hattop_t *state = (hattop_t*)malloc(sizeof(hattop_t));
    if(state) {
        state->sockfd = -1;
        state->portno = DEFAULT_HTTP_PORT;
    }
    return state;
}

void hattop_destroy(hattop_t *state)
{
    free((void*)state);
}

