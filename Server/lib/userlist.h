//
// Created by Kevin on 10/10/2015.
//

#ifndef HANGMAN_LIST_H
#define HANGMAN_LIST_H

#include "game.h"
#include "user.h"

typedef struct Game Game;

typedef struct UserList {
    int count;
    User **users;
} UserList;

void UserList_add(User *user);
User * UserList_get(char *name);
int     UserList_index(char *name);
void    UserList_move_user(User *u, int pos);

UserList *newUserList();

#endif //HANGMAN_LIST_H