//
// Created by Kevin on 10/10/2015.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "game.h"
#include "control.h"

Game *newGame(int id) {
    Game *g = malloc(sizeof(Game));
    g->id = id;
    return g;
}

void Game_board(Game *g) {

}