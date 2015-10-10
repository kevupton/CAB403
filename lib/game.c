//
// Created by Kevin on 10/10/2015.
//

#include "game.h"

Game *newGame(int id) {
    Game *g = malloc(sizeof(Game));
    g->id = id;
    return g;
}