//
// Created by Kevin on 10/10/2015.
//

#ifndef HANGMAN_CONNECTION_H
#define HANGMAN_CONNECTION_H

#include "control.h"

static const int DATA_LENGTH;

typedef struct Connection {
    char *ip_address;
    int port;
    int _sock;
    pthread_t listener;
} Connection;

Connection *newConnection(char *ip, char *port);
int Connection_initialise();
void Connection_close();
void *Connection_listen(void *p);
int Connection_send(char *msg);
void Connection_login(char *username, char *password);
void Connection_play();
void Connection_leaderboard();

char *_prepare_msg(int len, ...);

#endif //HANGMAN_CONNECTION_H
