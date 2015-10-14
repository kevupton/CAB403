//
// Created by Kevin on 10/10/2015.
//

#ifndef HANGMAN_CONTROL_H
#define HANGMAN_CONTROL_H

#include "list.h"
#include "game.h"
#include "connection.h"
#include "event.h"
#include "leaderboard.h"
#include "instance.h"
#include "user.h"

extern const int MAX_PLAYERS;

typedef struct Connection Connection;
typedef struct List List;

typedef struct Control {
    Connection *conn;
    List *users;
    List *instances;
    List *auth;
    List *words;
    pthread_t _control_thread;
} Control;

Control *newControl(char *argv[]);
Control *control;
void Controller_run();

void _boot(Control *control);
void _start_worker(Control *control);
void _load_authentication(Control *control);
void _load_words(Control *control);

#endif //HANGMAN_CONTROL_H
