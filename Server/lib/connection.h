//
// Created by Kevin on 10/10/2015.
//

#ifndef HANGMAN_CONNECTION_H
#define HANGMAN_CONNECTION_H

#include "control.h"

static const int DATA_LENGTH;

typedef struct Connection {
    int port;
    pthread_t *active_threads;
} Connection;

Connection *newConnection(char *port);
int Connection_initialise(Connection *c);
void Connection_close(Connection *c);
void *Connection_listen(Connection *c);
int Connection_send(Connection *c, char *msg);
void *Connection_handler(void *socket_desc);
char *_prepare_msg(int len, ...);

#endif //HANGMAN_CONNECTION_H
