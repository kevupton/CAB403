//
// Created by Kevin on 10/10/2015.
//

#include <stdio.h>
#include <stdlib.h>
#include "control.h"


Control *newControl(char *argv[]) {
    Control *c = malloc(sizeof(Control));
    //c->conn = newConnection(argv[1]);
    c->userList = newUserList();
    return c;
}