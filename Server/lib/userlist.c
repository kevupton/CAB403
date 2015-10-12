//
// Created by Kevin on 10/10/2015.
//

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "userlist.h"

void UserList_add(User *user) {
    if (UserList_exists(user->username)) {
        puts("Game already exists");
    } else {
        puts("processing");
        size_t new_size = control->users->count + 1;

        User *new_users = realloc(control->users->users, new_size);
        if (new_users) {
            control->users->users = new_users;
            control->users->users[control->users->count] = user;
            control->users->count += 1;
        } else {
            puts("Error users size");
            // deal with realloc failing because memory could not be allocated.
        }
    }
}

int UserList_exists(char *name) {
    int i = 0;
    for (i; i < control->users->count; i += 1) {
        if (equals(lowercase(control->users->users[i]->username), lowercase(name))) return i;
    }
    return -1;
}

User *UserList_get(char *name) {
    int index;
    if ((index = UserList_exists(name)) != -1) {
        return control->users->users[index];
    }
}

UserList *newUserList() {
    UserList *u = malloc(sizeof(UserList));

    u->users = malloc(0);
    u->count = 0;

    return u;
}

