//
// Created by Kevin on 10/10/2015.
//

#ifndef HANGMAN_LIST_H
#define HANGMAN_LIST_H

#include "game.h"

typedef struct Game Game;
typedef struct Control Control;
typedef struct GameList {
    int count;
    int *ids;
    Game **games;
} GameList;

void    GameList_add(GameList *g, int id, Game *game);
Game   *GameList_get(GameList *g, int id);
int     GameList_exists(GameList *g, int id);
GameList *newGameList();

#endif //HANGMAN_LIST_H