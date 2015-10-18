//
// Created by Kevin on 10/10/2015.
//

#include <stdio.h>
#include "control.h"

/**
 * Creates an instance of a new control.
 * Registers the connections on instantiation.
 *
 * @param argv the list of arguments from the command line.
 */
Control *newControl(char *argv[]) {
    Control *c = malloc(sizeof(Control));
    c->_do_wait = 1;
    c->_is_waiting = 0;

    //register the connection
    c->conn = newConnection(argv[1], argv[2]);

    c->game = NULL;
    c->keep_alive = 1;

    sem_init(&c->sem_listen, 0, 0);

    return c;
}

/**
 * Exits the program
 */
void Control_exit() {
    Connection_close();
    exit(0);
}

/**
 * Waits until the stop_waiting method is called
 */
void wait() {
    while (control->_do_wait) {sem_wait(&control->sem_listen);}
    control->_do_wait = 1;
}

/**
 * Releases the program from the wait method.
 */
void stop_waiting() {
    control->_do_wait = 0;
    sem_post(&control->sem_listen);
}