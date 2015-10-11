//
// Created by Kevin on 10/10/2015.
//

#ifndef HANGMAN_GAME_H
#define HANGMAN_GAME_H

#include <stdlib.h>
#include "control.h"

typedef struct Control Control;
typedef struct Game {
    int id;
    char *guesses;
    int nb_left;
    char **word;
} Game;

Game *newGame(int id);
void Game_initialise();
void Game_board();
void Game_welcome();
int Game_login();
int _valid_login(char *username, char *password);

#endif //HANGMAN_GAME_H
