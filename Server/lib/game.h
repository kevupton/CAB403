//
// Created by Kevin on 10/10/2015.
//

#ifndef HANGMAN_GAME_H
#define HANGMAN_GAME_H

#include "control.h"

typedef struct Control Control;

static const int STARTING_GUESSES;

typedef struct Game {
    int id;
    char *guesses;
    int nb_left;
    char **word;
} Game;

Game *newGame(int id);
void Game_initialise();
void Game_board(Game *g);

#endif //HANGMAN_GAME_H
