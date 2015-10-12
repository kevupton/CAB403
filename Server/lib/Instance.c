//
// Created by Kevin on 13/10/2015.
//

#include <stdlib.h>
#include <stdio.h>
#include "Instance.h"

Instance *newInstance() {
    int *new_sock;
    Instance *i = malloc(sizeof(Instance));
    pthread_t sniffer_thread;

    if( pthread_create( &sniffer_thread , NULL ,  Connection_handler , (void*) new_sock) < 0)
    {
        perror("could not create thread");
        return NULL;
    }

    i->_thread = sniffer_thread;
    i->_sock = 0;
    i->user = NULL;
    i->game = NULL;

    return i;
}