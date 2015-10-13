//
// Created by Kevin on 10/10/2015.
//

#ifndef HANGMAN_GAME_H
#define HANGMAN_GAME_H

#include "control.h"

typedef struct Control Control;

static const int STARTING_GUESSES;

typedef struct Game {
    char *guesses;
    int nb_left;
    char **words;
    char **visible;
    int word_a;
    int word_b;
    int index;
    int nb_guesses;
} Game;

Game *newGame(int *prev_index);
void ** _get_random_pair(const int prev_index, int *new_index);
void Free_game(Game **g);

#endif //HANGMAN_GAME_H
