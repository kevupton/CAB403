//
// Created by Kevin on 10/10/2015.
//

#include <stdio.h>
#include "control.h"


Control *newControl() {
    Control *c = malloc(sizeof(Control));
    c->games = newGameList();
    return c;
}