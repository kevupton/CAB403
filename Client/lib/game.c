//
// Created by Kevin on 10/10/2015.
//

#include <stdio.h>
#include <string.h>
#include "game.h"
#include "control.h"

Game *newGame(int id) {
    Game *g = malloc(sizeof(Game));
    g->id = id;
    return g;
}

void Game_initialise(Control *c) {
    char cc[1000];
    while (1) {
        printf("Enter Character : ");
        scanf("%s" , cc);
        puts(cc);

        Connection_send(c->conn, cc);

        if (strcmp(cc, "exit") == 0) {
            break;
        }
    }
}

void Game_board(Game *g) {

}