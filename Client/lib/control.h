//
// Created by Kevin on 10/10/2015.
//

#ifndef HANGMAN_CONTROL_H
#define HANGMAN_CONTROL_H

#include "game.h"
#include "connection.h"
#include "event.h"

typedef struct Connection Connection;
typedef struct Game Game;
typedef struct Control {
    Game *game;
    Connection *conn;
    volatile int keep_alive;
    char *username;
    volatile int _login_received;
    volatile int _game_setup;
    volatile int _game_guessing;
} Control;

Control *newControl(char *argv[]);
void Control_exit();

Control *control;

#endif //HANGMAN_CONTROL_H
