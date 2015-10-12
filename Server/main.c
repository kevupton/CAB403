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

        int toget = 11;

        Leaderboard *l = newLeaderboard(0, 10);

        int i;
        for (i = 0; i < toget; i++) {
            printf("%s got a score of %d\n", l->names[i], l->wins[i]);
        }

        UserList_add(newUser("kevin"));
        UserList_add(newUser("kevin"));
        UserList_add(newUser("Bob"));

        printf("\n\n%d", control->userList->count);

    }
    return 0;
}