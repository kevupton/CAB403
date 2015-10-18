//
// Created by Kevin on 17/10/2015.
//

#include <stdlib.h>
#include <stdio.h>
#include "leaderworker.h"

/**
 * Creates a new LeaderWorker instance, that looks after changing the results around in
 * the leaderboard. Creates a new thread which all this happens on.
 *
 * @return LeaderWorker* the instance of the LeaderWorker
 */
LeaderWorker *newLeaderWorker() {
    LeaderWorker *l = malloc(sizeof(LeaderWorker));
    l->_run = 1;
    l->results = newList(sizeof(User*) + sizeof(int*));
    sem_init(&l->listener, 0, 0);

    //attempt to make a new thread.
    if( pthread_create( &l->_thread, NULL ,  Leaderboard_worker , l) < 0)
    {
        perror("could not create Leaderboard Worker");
    }

    return l;
}

/**
 * The main function which handles what the worker does.
 * Gets all pending result insertions and inserts them into the user,
 * afterwards rearranging them in the array list so that they are in order
 * for the leaderboard.
 *
 * @param p
 *
 * @return void*
 */
void *Leaderboard_worker(void *p) {
    LeaderWorker *l = p;
    void **results;
    User *u;
    int win;

    while (l->_run) { //while leaderworker is told to run
        while (l->results->count > 0) { //and there are results to do
            //get the next result in the list. (popping it off the bottom)
            results = List_pop(l->results);

            u = results[0]; //the user
            win = *(int *) results[1]; //whether it won or not

            u->played++;
            u->won += win;

            //update the user with its new position (if it has one)
            _update_user(u);
            free(results);
        }
        sem_wait(&l->listener);
    }
}

/**
 * Adds a user to the queue for the leaderboard insertion.
 * This queue is processed by the leaderboard worker.
 *
 * @param user the user to be updated.
 * @param win whether the user won or not.
 */
void Leaderboard_result(User *user, int *win) {
    //create a new container of memory
    void **ptr = malloc((size_t) control->leaderWorker->results->_type_size);

    ptr[0] = user;
    ptr[1] = win;

    List_add(control->leaderWorker->results, ptr); //add the container to the memory.
    sem_post(&control->leaderWorker->listener); //wake up the worker.
}

/**
 * Close the LeaderWorker. Stops any further results being added.
 */
void LeaderWorker_close() {
    control->leaderWorker->_run = 0;
    sem_post(&control->leaderWorker->listener);
}