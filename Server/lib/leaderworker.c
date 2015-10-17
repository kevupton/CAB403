//
// Created by Kevin on 17/10/2015.
//

#include <stdlib.h>
#include <stdio.h>
#include "leaderworker.h"

LeaderWorker *newLeaderWorker() {
    LeaderWorker *l = malloc(sizeof(LeaderWorker));
    l->_run = 1;
    l->results = newList(sizeof(User*) + sizeof(int*));
    sem_init(&l->listener, 0, 0);

    if( pthread_create( &l->_thread, NULL ,  Leaderboard_worker , l) < 0)
    {
        perror("could not create Leaderboard Worker");
    }

    return l;
}

void *Leaderboard_worker(void *p) {
    LeaderWorker *l = p;
    void **results;
    User *u;
    int win;

    while (l->_run) {
        while (l->results->count > 0) {
            results = List_pop(l->results);

            u = results[0];
            win = *(int *) results[1];

            u->played++;
            u->won += win;

            _update_user(u);
            free(results);
        }
        sem_wait(&l->listener);
    }
}

void Leaderboard_result(User *user, int *win) {
    void **ptr = malloc((size_t) control->leaderWorker->results->_type_size);

    ptr[0] = user;
    ptr[1] = win;

    List_add(control->leaderWorker->results, ptr);
    sem_post(&control->leaderWorker->listener);
}

void LeaderWorker_close() {
    control->leaderWorker->_run = 0;
    sem_post(&control->leaderWorker->listener);
}