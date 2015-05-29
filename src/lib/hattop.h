#ifndef HATTOP_H
#define HATTOP_H

/* Request Handler */
typedef void (*handler_t)(void *connection, const char *uri);

/* Webserver state */
typedef struct hattop_t
{
    handler_t handler;
} hattop_t;

/* Create an web server instance */
hattop_t *hattop_create();

/* Destroy web server instance */
void hattop_destroy(hattop_t *state);

/* Install a handler for incoming requests */
void hattop_register_handler(hattop_t *state, handler_t handler);

/* Start listening for connections */
int hattop_listen(hattop_t *state, short portno);

/* Send HTTP response to client */
void hattop_response_simple(void *connection, const char *content_type, const char *body, int body_len);

/* Send HTTP error to client */
void hattop_response_error(void *connection, int status);

#endif

