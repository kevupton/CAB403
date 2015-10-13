//
// Created by Kevin on 14/10/2015.
//

#include <stdlib.h>
#include "leaderboard.h"

Leaderboard *newLeaderboard(const int count) {
    Leaderboard *l = malloc(sizeof(Leaderboard));
    l->names = malloc(count * sizeof(char *));
    l->played = malloc(count * sizeof(int));
    l->wins = malloc(count * sizeof(int));
    l->count = count;
}

void Free_leaderboard(Leaderboard **l) {
    if (*l != NULL) {
        int i;
        for (i = 0; i < (*l)->count; i++) {
            free((*l)->names[i]);
        }
        free((*l)->names);
        free((*l)->played);
        free((*l)->wins);
        free((*l));
        (*l) = NULL;
    }
}