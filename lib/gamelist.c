//
// Created by Kevin on 10/10/2015.
//

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "gamelist.h"

void GameList::add(int id, Game game) {
    if (exists(id)) {
        puts("Game already exists");
    } else {
        size_t new_size = _count + 1;
        int* new_int = realloc(_ids, new_size);
        if (new_int) {
            _ids = new_int;
        } else {
            puts("Error new Int");
            // deal with realloc failing because memory could not be allocated.
        }
        Game *new_games = realloc(_games, new_size);
        if (new_games) {
            _ids = new_int;
        } else {
            puts("Error games size");
            // deal with realloc failing because memory could not be allocated.
        }
        _ids[_count] = id;
        _games[_count] = game;
        _count = new_size;
    }
}

int GameList::exists(int id) {
    for (int i = 0; i < _count; i++) {
        if (_ids[i] == id) return 1;
    }
    return 0;
}


Game GameList::get(int id) {
    if (exists(id)) {
        for (int i = 0; i < _count; i++) {
            if (_ids[i] == id) {
                return _games[i];
            }
        }
    }
}

