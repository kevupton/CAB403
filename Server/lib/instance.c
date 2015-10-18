//
// Created by Kevin on 13/10/2015.
//

#include <stdlib.h>
#include <stdio.h>
#include "Instance.h"

/**
 * Creates a new instance with its own thread and data associated.
 *
 * @return Instance* the new instance
 */
Instance *newInstance() {
    Instance *i = malloc(sizeof(Instance));

    i->game = NULL;
    i->user = NULL;
    i->keep_alive = 1;

    //initialise the listener
    sem_init(&i->sem_listener, 0, 0);

    Instance_reset(i);

    //create the thread
    if( pthread_create( &i->_thread , NULL ,  Connection_handler , i) < 0)
    {
        perror("could not create thread");
        return NULL;
    }

    return i;
}

/**
 * Gets an available instance, from the list of register instances.
 *
 * @return Instance* the instance or NULL if not found.
 */
Instance *Instance_get_available() {
    void **items = control->instances->items;
    int i = 0, len = control->instances->count;
    Instance *instance;

    for (i = 0; i < len; i++) { //loop through each instance
        instance = (Instance *) items[i];
        if (!instance->in_use) { //if the instance is not in use
            return instance;
        }
    }

    return NULL;
}

/**
 * Resets the data associated with the instance,
 * freeing any game data or user data or connection attached to it.
 *
 * @param i the instance to reset
 */
void Instance_reset(Instance *i) {
    i->_sock = 0;
    i->user = NULL;
    Free_game(&i->game);
    i->in_use = 0;
    i->prev_game_index = -1;
}


/**
 * Puts an instance to sleep
 *
 * @param i the instance to sleep.
 */
void Instance_sleep(Instance *i) {
    sem_wait(&i->sem_listener);
}

/**
 * Assigns a client to a an instance.
 * Waking up the instance and getting it back to work.
 *
 * @param i the instance to assign to
 * @param client_sock the socket to attach to the instance.
 */
void Instance_assign(Instance *i, int client_sock) {
    i->_sock = client_sock;
    i->in_use = 1;
    sem_post(&i->sem_listener);
}

/**
 * Wakes up an instance to get it running briefly.
 *
 * @param i the instnace to briefly wakeup.
 */
void Instance_wake(Instance *i) {
    sem_post(&i->sem_listener);
}