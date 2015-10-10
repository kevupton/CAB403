//
// Created by Kevin on 10/10/2015.
//

#ifndef HANGMAN_CONNECTION_H
#define HANGMAN_CONNECTION_H

#include "control.h"

typedef struct Connection {
    char *ip_address;
    int port;
    int _sock;
    pthread_t listener;
} Connection;

Connection *newConnection(char *ip, char *port);
int Connection_initialise(Connection *c);
void Connection_close(Connection *c);
void *Connection_listen(Connection *c);
int Connection_send(Connection *c, char *msg);

#endif //HANGMAN_CONNECTION_H
