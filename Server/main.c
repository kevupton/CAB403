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
        sleep(300);
//        int toget = 11;
//
//        User *kevin = newUser("kevin");
//        User *bob = newUser("bob");
//        User *joe = newUser("joe");
//        User *adam = newUser("adam");
//        User *krissy = newUser("krissy");
//
//        List_add(kevin);
//        List_add(bob);
//        List_add(joe);
//        List_add(adam);
//        List_add(krissy);
//
//        puts("starting");
//
//        joe->won = 1;
//        joe->played = 5;
//        _update_user(joe);
//
//        bob->won = 2;
//        bob->played = 4;
//        _update_user(bob);
//
//        adam->won = 7;
//        adam->played = 8;
//        _update_user(adam);
//
//        krissy->won = 6;
//        krissy->played = 8;
//        _update_user(krissy);
//
//        kevin->won = 6;
//        kevin->played = 9;
//        _update_user(kevin);
//
//        puts("updating");
//
//
//        printf("list = %d\n", control->users->count);
//
//        Leaderboard *l = newLeaderboard(0, 10);
//
//        int i;
//        for (i = 0; i < l->count; i++) {
//            printf("%s - %d - %d\n", l->names[i], l->played[i], l->wins[i]);
//        }
//
//        printf("\n\n%d", control->users->count);

    }
    return 0;
}