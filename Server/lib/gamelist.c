//
// Created by Kevin on 10/10/2015.
//

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "gamelist.h"

void GameList_add(GameList *g, int id, Game *game) {
    if (GameList_exists(g, id)) {
        puts("Game already exists");
    } else {
        puts("processing");
        size_t new_size = g->count + 1;
        if (g->count == 0) {
            g->ids = malloc(sizeof(int));
            g->games = malloc(sizeof(Game));
        } else {
            int* new_int = realloc(g->ids, new_size);
            if (new_int) {
                g->ids = new_int;
            } else {
                puts("Error new Int");
                // deal with realloc failing because memory could not be allocated.
            }
            Game *new_games = realloc(g->games, new_size);
            if (new_games) {
                g->ids = new_int;
            } else {
                puts("Error games size");
                // deal with realloc failing because memory could not be allocated.
            }
        }
        g->ids[g->count] = id;
        g->games[g->count] = game;
        g->count += 1;
    }
}

int GameList_exists(GameList *g, int id) {
    int i = 0;
    for (i; i < g->count; i += 1) {
        if (g->ids[i] == id) return 1;
    }
    return 0;
}

Game *GameList_get(GameList *g, int id) {
    if (GameList_exists(g, id)) {
        int i = 0;
        for (i; i < g->count; i += 1) {
            if (g->ids[i] == id) {
                return g->games[i];
            }
        }
    }
}

GameList *newGameList() {
    GameList *gl = malloc(sizeof(GameList));
    Game games[0];
    int ids[0];

    gl->games = &games;
    gl->ids = &ids;
    gl->count = 0;

    return gl;
}

