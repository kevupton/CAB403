//
// Created by Kevin on 13/10/2015.
//

#ifndef SERVER_INSTANCE_H
#define SERVER_INSTANCE_H

#include <pthread.h>
#include "game.h"
#include "user.h"

typedef struct Game Game;

typedef struct Instance {
    volatile Game *game;
    volatile User *user;
    volatile int _sock;
    const pthread_t *_thread;
} Instance;

Instance *newInstance();


#endif //SERVER_INSTANCE_H
