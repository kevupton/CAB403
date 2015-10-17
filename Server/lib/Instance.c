//
// Created by Kevin on 13/10/2015.
//

#include <stdlib.h>
#include <stdio.h>
#include "Instance.h"

Instance *newInstance() {
    Instance *i = malloc(sizeof(Instance));
    pthread_t sniffer_thread;
    i->game = NULL;
    i->user = NULL;
    i->keep_alive = 1;

    sem_init(&i->sem_listener, 0, 0);

    Instance_reset(i);

    if( pthread_create( &sniffer_thread , NULL ,  Connection_handler , i) < 0)
    {
        perror("could not create thread");
        return NULL;
    }

    i->_thread = sniffer_thread;

    return i;
}

Instance *Instance_get_available() {
    void **items = control->instances->items;
    int i = 0, len = control->instances->count;
    Instance *instance;

    for (i = 0; i < len; i++) {
        instance = (Instance *) items[i];
        if (!instance->in_use) {
            return instance;
        }
    }

    return NULL;
}

void Instance_reset(Instance *i) {
    i->_sock = 0;
    i->user = NULL;
    Free_game(&i->game);
    i->in_use = 0;
    i->prev_game_index = -1;
}

void Instance_sleep(Instance *i) {
    sem_wait(&i->sem_listener);
}

void Instance_assign(Instance *i, int client_sock) {
    i->_sock = client_sock;
    i->in_use = 1;
    sem_post(&i->sem_listener);
}