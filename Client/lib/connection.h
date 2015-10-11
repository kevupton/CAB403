//
// Created by Kevin on 10/10/2015.
//

#ifndef HANGMAN_CONNECTION_H
#define HANGMAN_CONNECTION_H

#include "control.h"

static const int DATA_LENGTH;
static const int DATA_SIZE;

typedef struct Connection {
    char *ip_address;
    int port;
    int _sock;
    pthread_t listener;
} Connection;

Connection *newConnection(char *ip, char *port);
int Connection_initialise();
void Connection_close();
void *Connection_listen();
int Connection_send(char *msg);
int Connection_login(char *username, char *password);

char *_prepare_msg(char *route, char **data, int len);

#endif //HANGMAN_CONNECTION_H
