//
// Created by Kevin on 10/10/2015.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "game.h"
#include "control.h"

const int LEADERBOARD_SHOW = 10;

/**
 * Creates a new game instance not the same as the last game words.
 *
 * @param prev_index a reference to the int containing the previous location
 *
 * @return Game* a new game instance.
 */
Game *newGame(int *prev_index) {
    Game *g = malloc(sizeof(Game));
    int nb_left = 0;

    //gets the pair of words.
    g->words = (char **) _get_random_pair((*prev_index), &g->index);
    (*prev_index) = g->index; //assign the new prev_game index

    //calculate the number left
    nb_left = (int) (strlen(g->words[0]) + strlen(g->words[1]) + 10);
    g->nb_left = (nb_left > 26)? 26: nb_left;

    g->nb_guesses = 0;
    g->word_a = (int) strlen(g->words[0]);
    g->word_b = (int) strlen(g->words[1]);

    //allocate memory to the game data
    g->guesses = calloc(sizeof(char), (size_t) (nb_left + 1));
    g->visible = malloc(2*sizeof(char*));
    g->visible[0] = calloc(sizeof(char), (size_t) g->word_a + 1);
    g->visible[1] = calloc(sizeof(char), (size_t) g->word_b + 1);

    //default the visible words to underscores.
    memset(g->visible[0], '_', (size_t) g->word_a);
    memset(g->visible[1], '_', (size_t) g->word_b);

    return g;
}

/**
 * Gets a random pair of words from the stored control word list.
 *
 * @param prev_index the previous index
 * @param new_index where to assign the new index
 */
void ** _get_random_pair(const int prev_index, int *new_index) {
    int index = -1;

    /** Random seed initialisation **/
    srand((unsigned) time(NULL));

    do { //get a new random index
        index = rand() % control->words->count;
    } while(index == prev_index); //and repeat until its different from the last
    (*new_index) = index;

    //return the pair
    return List_get(control->words, index);
}

/**
 * Free the data associated with the game.
 *
 * @param g the game instance to free up.
 */
void Free_game(Game **g) {
    if (*g != NULL) {
        free((*g)->guesses);
        free((*g)->visible[0]);
        free((*g)->visible[1]);
        free((*g)->visible);
        free((*g));
        *g = NULL;
    }
}