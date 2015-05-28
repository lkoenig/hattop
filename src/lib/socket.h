#ifndef SOCKET_H
#define SOCKET_H

typedef int socket_t;

/* Create server socket listening on portno */
socket_t SOCKET_create(short portno);

/* Accept incoming connection or time out */
socket_t SOCKET_accept(socket_t s, int timeout_ms);

/* Check if socket is valid */
int SOCKET_is_valid(socket_t s);

/* Send data over socket */
void SOCKET_send(socket_t s, const char *buf, int buflen);

/* Receive data from socket */
int SOCKET_recv(socket_t s, char *buf, int bufsize);

/* Close socket */
void SOCKET_close(socket_t s);

#endif
