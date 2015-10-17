//
// Created by Kevin on 17/10/2015.
//

#ifndef SERVER_LEADERWORKER_H
#define SERVER_LEADERWORKER_H

#include <semaphore.h>
#include "list.h"

typedef struct List List;

typedef struct LeaderWorker {
    volatile int _run;
    sem_t listener;
    pthread_t _thread;
    List *results;
} LeaderWorker;

LeaderWorker *newLeaderWorker();

void *Leaderboard_worker(void *p);
void LeaderWorker_close();
void Leaderboard_result(User *user, int *i);

#endif //SERVER_LEADERWORKER_H
