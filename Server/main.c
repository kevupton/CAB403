//
// Created by Kevin on 10/10/2015.
//

#include <stdio.h>
#include "lib/control.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        puts("Server requires a Port number");
    } else {
        control = newControl(argv);
        Leaderboard_add("kevin", 7);
        Leaderboard_add("kevin", 7);
        Leaderboard_add("kevin", 7);
        Leaderboard_add("kevin", 7);
        Leaderboard_add("kevin", 7);
        Leaderboard_add("kevin", 7);
        Leaderboard_add("kevin", 7);
        Leaderboard_add("kevin", 7);
        Leaderboard_add("kevin", 1);
        Leaderboard_add("kevin", 6);

        int toget = 11;

        void **results = Leaderboard_get(0, toget);

        char **names = results[0];
        int *scores = results[1];
        int i;
        for (i = 0; i < toget; i++) {
            printf("%s got a score of %d\n", names[i], scores[i]);
        }
    }
    return 0;
}