//
// Created by Kevin on 10/10/2015.
//

#ifndef HANGMAN_CONTROL_H
#define HANGMAN_CONTROL_H

#include "gamelist.h"
#include "game.h"
#include "connection.h"

typedef struct Connection Connection;

typedef struct {
    Game *game;
    Connection *conn;
} Control;

Control *newControl(char *argv[]);

#endif //HANGMAN_CONTROL_H
