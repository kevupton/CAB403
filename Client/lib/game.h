//
// Created by Kevin on 10/10/2015.
//

#ifndef HANGMAN_GAME_H
#define HANGMAN_GAME_H

#include <stdlib.h>
#include "control.h"

typedef enum {
    _,
    PLAY_HANGMAN,
    SHOW_LEADERBOARD,
    QUIT
} Menu;

typedef struct Control Control;
typedef struct Game {
    char *guesses;
    int nb_left;
    char **words;
} Game;

Game *newGame(const int word_a, const int word_b, const int nb_guesses);
void Game_initialise();
void Game_welcome();
void Game_menu();
void Game_play_hangman();
void Game_show_leaderboard();
int _menu_input();
void Game_login();
void Free_game(Game *g);

#endif //HANGMAN_GAME_H
