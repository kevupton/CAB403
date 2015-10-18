//
// Created by Kevin on 12/10/2015.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "user.h"
#include "control.h"

/**
 * Creates a new User instance with the specified name.
 * references the source of the name.
 *
 * @param user the original location of the name.
 *
 * @return User* the new user.
 */
User *newUser(char *user) {
    User *u = malloc(sizeof(User));

    //define the attributes.
    u->username = user;
    u->played = 0;
    u->won = 0;

    return u;
}

/**
 * Finds a user in the list of Users that have logged on before.
 * Returns the instance of the user.
 *
 * @param username the name to search for.
 *
 * @return User* the user.
 */
User *User_find(char *username) {
    int i;
    User *u;
    char *lower_a = lowercase(username), *lower_b;

    for (i = 0; i < control->users->count; i++) { //search all instances.
        u = List_get(control->users, i);
        lower_b = lowercase(u->username);
        if (strcmp(lower_a, lower_b) == 0) {
            free(lower_a);
            free(lower_b);
            return u;
        }
        free(lower_b);
    }
    free(lower_a);
    return NULL;
}

/**
 * Logs a user into the system, either creating a new User instance,
 * if that have not logged into the system before, or reuses the previously
 * used User instance.
 *
 * @param username the original source of the name
 */
User *User_login(char *username) {
    User *u = User_find(username);
    if (u == NULL) {
        u = newUser(username);
        List_add(control->users, u);
    }
    return u;
}