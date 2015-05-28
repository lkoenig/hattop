#ifndef HATTOP_H
#define HATTOP_H

/* Webserver state */
struct hattop_t;
typedef struct hattop_t hattop_t;

/* Create an web server instance */
hattop_t *hattop_create();

/* Destroy web server instance */
void hattop_destroy(hattop_t *state);

#endif

