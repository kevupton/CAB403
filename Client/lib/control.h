//
// Created by Kevin on 10/10/2015.
//

#ifndef HANGMAN_CONTROL_H
#define HANGMAN_CONTROL_H

#include <semaphore.h>
#include "game.h"
#include "connection.h"
#include "event.h"
#include "leaderboard.h"

typedef struct Connection Connection;
typedef struct Game Game;
typedef struct Control {
    Game *game;
    Connection *conn;
    Leaderboard *leaderboard;
    volatile int keep_alive;
    volatile int _do_wait;
    volatile int _is_waiting;

    char *username;

    sem_t sem_listen;
} Control;

Control *newControl(char *argv[]);
void Control_exit();
void wait();
void stop_waiting();

Control *control;

#endif //HANGMAN_CONTROL_H
