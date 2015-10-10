//
// Created by Kevin on 10/10/2015.
//

#include <stdio.h>
#include "lib/control.h"

int main() {
    puts("TEST");
    Control *control = newControl();
    puts("Started");

    Game *g1 = newGame(100);

    puts("HERE");

    GameList_add(&control->games, 10, g1);
    g1 = GameList_get(&control->games, 10);

    //puts(g1->id + "");
    puts("\nTEST");
    return 0;
}