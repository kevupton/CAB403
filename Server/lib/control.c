//
// Created by Kevin on 10/10/2015.
//

#include <stdio.h>
#include <stdlib.h>
#include "control.h"


static const int MAX_PLAYERS = 10;

Control *newControl(char *argv[]) {
    Control *c = malloc(sizeof(Control));
    c->conn = newConnection(argv[1]);
    c->users = newList(sizeof(User *));
    c->instances = newList(sizeof(Instance*));

    _boot(c);

    return c;
}

void _boot(Control *control) {
    int i;
    Instance *instance;

    for (i = 0; i < MAX_PLAYERS; i++) {
        instance = newInstance();
        List_add(control->instances, instance);
    }

    _start_worker(control);
}

void _start_worker(Control *control) {
    //Accept and incoming connection
    puts("Waiting for incoming connections...");

    int *new_sock;
    pthread_t thread;

    if( pthread_create( &thread, NULL ,  Connection_listen , control->conn) < 0)
    {
        perror("could not create thread");
        return;
    }

    control->_control_thread = thread;
}

void Controller_run() {
    while (1) {

    }
}