//
// Created by Kevin on 10/10/2015.
//

#include <stdio.h>
#include <sys/unistd.h>
#include <stdlib.h>
#include "lib/control.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        puts("Server requires a Port number");
    } else {
        control = newControl(argv);

        User *u = User_login("Maolin");
        User *j = User_login("Jason");
        User *a = User_login("Anna");
        User *p = User_login("Peter");

        int i;
        Leaderboard *l;

        int win = 1; int lose = 0;
        Leaderboard_result(a, &lose);
        Leaderboard_result(a, &lose);
        Leaderboard_result(u, &win);
        Leaderboard_result(a, &win);
        Leaderboard_result(u, &lose);
        sleep(2);
        l = newLeaderboard(0, 10);
        for (i = l->count - 1; i >= 0; i--) {
            printf("%d: %s %d %d\n", i, l->names[i], l->played[i], l->wins[i]);
        }
        sleep(4);
        puts("\n");
        Leaderboard_result(j, &win);
        Leaderboard_result(j, &win);
        Leaderboard_result(j, &win);
        sleep(1);

        l = newLeaderboard(0, 10);
        for (i = l->count - 1; i >= 0; i--) {
            printf("%d: %s %d %d\n", i, l->names[i], l->played[i], l->wins[i]);
        }
        puts("\n");
        Leaderboard_result(p, &win);
        Leaderboard_result(p, &win);
        Leaderboard_result(p, &win);
        Leaderboard_result(p, &lose);
        Leaderboard_result(p, &lose);
        Leaderboard_result(p, &lose);
        Leaderboard_result(p, &win);

        sleep(1);

        l = newLeaderboard(0, 10);
        for (i = l->count - 1; i >= 0; i--) {
            printf("%d: %s %d %d\n", i, l->names[i], l->played[i], l->wins[i]);
        }

        exit(0);

        if (control->conn) {
            Controller_run();
        } else {
            puts("Could not connect");
        }
    }
    return 0;
}