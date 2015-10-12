//
// Created by Kevin on 10/10/2015.
//

#ifndef HANGMAN_CONTROL_H
#define HANGMAN_CONTROL_H

#include "userlist.h"
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
    UserList *users;
} Control;

Control *newControl(char *argv[]);
Control *control;

#endif //HANGMAN_CONTROL_H
