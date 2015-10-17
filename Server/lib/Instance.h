//
// Created by Kevin on 13/10/2015.
//

#ifndef SERVER_INSTANCE_H
#define SERVER_INSTANCE_H

#include <pthread.h>
#include <semaphore.h>
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

    sem_t sem_listener;
    pthread_t _thread;
} Instance;

Instance *newInstance();
Instance *Instance_get_available();
void Instance_reset(Instance *i);
void Instance_sleep(Instance *i);
void Instance_wake(Instance *i);
void Instance_assign(Instance *i, int client_sock);
#endif //SERVER_INSTANCE_H
