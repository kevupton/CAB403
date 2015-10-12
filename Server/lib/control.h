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
#include "instance.h"
#include "user.h"

typedef struct Connection Connection;
typedef struct UserList UserList;

typedef struct Control {
    Connection *conn;
    UserList *userList;
} Control;

Control *newControl(char *argv[]);
Control *control;

#endif //HANGMAN_CONTROL_H
