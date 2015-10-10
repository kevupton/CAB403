//
// Created by Kevin on 10/10/2015.
//

#ifndef HANGMAN_GAME_H
#define HANGMAN_GAME_H

#include <stdlib.h>

typedef struct {
    int id;
} Game;

Game *newGame(int id);

#endif //HANGMAN_GAME_H
