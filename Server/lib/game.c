//
// Created by Kevin on 10/10/2015.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "game.h"
#include "control.h"

Game *newGame(int *prev_index) {
    Game *g = malloc(sizeof(Game));
    int nb_left = 0;

    g->words = (char **) _get_random_pair((*prev_index), &g->index);
    (*prev_index) = g->index;
    nb_left = (int) (strlen(g->words[0]) + strlen(g->words[1]) + 10);
    g->nb_left = (nb_left > 26)? 26: nb_left;
    g->guesses = "";
    g->nb_guesses = 0;

    return g;
}

void ** _get_random_pair(const int prev_index, int *new_index) {
    int index = -1;
    do {
        index = rand() % control->words->count;
    } while(index == prev_index);
    (*new_index) = index;
    return List_get(control->words, index);
}

void Free_game(Game *g) {
    if (g != NULL) {
        free(g->words[0]);
        free(g->words[1]);
        free(g->words);
        free(g->guesses);
        free(g);
        g = NULL;
    }
}