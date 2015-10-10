//
// Created by Kevin on 10/10/2015.
//

#ifndef HANGMAN_CONTROL_H
#define HANGMAN_CONTROL_H

#include "gamelist.h"
#include "game.h"

typedef struct {
    GameList *games;
} Control;

Control *newControl();

#endif //HANGMAN_CONTROL_H
