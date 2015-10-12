//
// Created by Kevin on 12/10/2015.
//

#include <stdlib.h>
#include "user.h"


User *newUser(char *user) {
    User *u = malloc(sizeof(User));

    u->username = user;
    u->played = 0;
    u->won = 0;

    return u;
}