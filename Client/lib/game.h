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
    int id;
    char *guesses;
    int nb_left;
    char **word;
} Game;

Game *newGame(int id);
void Game_initialise();
void Game_board();
void Game_welcome();
void Game_menu();
void Game_play_hangman();
void Game_show_leaderboard();
int _menu_input();
void Game_login();
void Game_parse_login(int success, char *username);

#endif //HANGMAN_GAME_H
