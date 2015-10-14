//
// Created by Kevin on 12/10/2015.
//

#include <stdlib.h>
#include <stdio.h>
#include "user.h"
#include "control.h"


User *newUser(char *user) {
    User *u = malloc(sizeof(User));

    u->username = user;
    u->played = 0;
    u->won = 0;

    return u;
}


User *User_find(char *username) {
    int i, len = control->users->count;
    User *u;

    for (i = 0; i < len; i++) {
        u = List_get(control->users, i);
        if (equals(lowercase(u->username), lowercase(username))) {
            return u;
        }
    }

    return NULL;
}

User *User_login(char *username) {
    User *u = User_find(username);
    if (u == NULL) {
        u = newUser(username);
        List_add(control->users, u);
    }
    return u;
}