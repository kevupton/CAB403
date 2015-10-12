//
// Created by Kevin on 10/10/2015.
//

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "userlist.h"

void UserList_add(User *user) {
    if (UserList_index(user->username) != -1) {
        puts("User already exists");
    } else {
        puts("processing");
        size_t new_size = control->userList->count + 1;
        User **new_users = realloc(control->userList->users, new_size * sizeof(User *));

        if (new_users) {
            control->userList->users = new_users;
            control->userList->users[control->userList->count] = user;
            control->userList->count += 1;
        } else {
            puts("Error userList size");
            // deal with realloc failing because memory could not be allocated.
        }
    }
}

int UserList_index(char *name) {
    int i = 0;
    for (i; i < control->userList->count; i += 1) {
        if (equals(lowercase(control->userList->users[i]->username), lowercase(name))) return i;
    }
    return -1;
}

User *UserList_get(char *name) {
    int index;
    if ((index = UserList_index(name)) != -1) {
        return control->userList->users[index];
    }
}

UserList *newUserList() {
    UserList *u = malloc(sizeof(UserList));

    u->users = malloc(0);
    u->count = 0;

    return u;
}

void UserList_move_user(User *u, int pos) {
    int index = UserList_index(u->username), len = control->userList->count, i;
    User **users = control->userList->users;

    if (pos != index) {
        if (pos < index) {
            for (i = index; i > pos; i--) {
                users[i] = users[i - 1];
            }
        } else {
            for (i = index; i < pos; i++) {
                users[i] = users[i + 1];
            }
        }
        users[pos] = u;
    }
}
