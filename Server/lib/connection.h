//
// Created by Kevin on 10/10/2015.
//

#ifndef HANGMAN_CONNECTION_H
#define HANGMAN_CONNECTION_H

#include "control.h"
#include "instance.h"

extern const int DATA_LENGTH;

typedef struct Instance Instance;

typedef struct Connection {
    int port;
    int _sock;
} Connection;

Connection *newConnection(char *port);
void *Connection_listen(void *conn);
void Connection_write(int sock, char *msg);
void *Connection_handler(void *socket_desc);
char *_prepare_msg(int len, ...);
char *_concat_array(int len, char **data, char *join);
#endif //HANGMAN_CONNECTION_H
