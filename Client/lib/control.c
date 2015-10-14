//
// Created by Kevin on 10/10/2015.
//

#include <stdio.h>
#include <sys/unistd.h>
#include "control.h"


Control *newControl(char *argv[]) {
    Control *c = malloc(sizeof(Control));
    c->conn = newConnection(argv[1], argv[2]);
    c->game = NULL;
    c->keep_alive = 1;
    return c;
}

void Control_exit() {
    Connection_close();
    exit(0);
}

void wait_for(int *_switch, int result) {
    while (*_switch != result) { sleep(1); }
}