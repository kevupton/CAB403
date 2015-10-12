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

        User *kevin = newUser("kevin");
        User *bob = newUser("bob");
        User *joe = newUser("joe");
        User *adam = newUser("adam");
        User *krissy = newUser("krissy");

        UserList_add(kevin);
        UserList_add(bob);
        UserList_add(joe);
        UserList_add(adam);
        UserList_add(krissy);

        puts("starting");

        bob->won = 2;
        bob->played = 4;
        _update_user(bob);

        joe->won = 1;
        joe->played = 5;
        _update_user(joe);

        adam->won = 7;
        adam->played = 8;
        _update_user(adam);


        puts("updating");


        printf("list = %d\n", control->userList->count);

        Leaderboard *l = newLeaderboard(0, 10);

        int i;
        for (i = 0; i < toget; i++) {
            printf("%s got a score of %d\n", l->names[i], l->wins[i]);
        }

        printf("\n\n%d", control->userList->count);

    }
    return 0;
}