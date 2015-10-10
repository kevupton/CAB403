//
// Created by Kevin on 10/10/2015.
//

#ifndef HANGMAN_LIST_H
#define HANGMAN_LIST_H

#include "game.h";

typedef struct GameList {
    int _count;
    int *_ids;
    Game *_games;

    void    add(int id, Game game);
    Game    get(int id);
    int     exists(int id);
} GameList;

#endif //HANGMAN_LIST_H