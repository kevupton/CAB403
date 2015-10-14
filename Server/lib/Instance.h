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
    Game *game;
    User *user;
    volatile int _sock;
    volatile int in_use;
    volatile int keep_alive;

    int prev_game_index;
    int _thread_index;

    pthread_t _thread;
} Instance;

Instance *newInstance();
Instance *Instance_get_available();
void Instance_reset(Instance *i);

#endif //SERVER_INSTANCE_H
