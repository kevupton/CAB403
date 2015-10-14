//
// Created by Kevin on 10/10/2015.
//

#include <stdio.h>
#include <sys/unistd.h>
#include <stdlib.h>
#include <string.h>
#include "lib/control.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        puts("Server requires a Port number");
    } else {
        //puts("test");
        control = newControl(argv);
//        char *name = calloc(sizeof(char), 10);
//        strcpy(name, "Maolin");
//
//        User *u = User_login(name);
//        u->played = 10;
//        u->won = 2;
//        _update_user(u);
//
//        User *m = User_login("Mike");
//        m->played = 4;
//        m->won = 4;
//        _update_user(m);
//
//        u->played++;
//        u->won++;
//        _update_user(u);
//
//        u->played++;
//        u->won++;
//        _update_user(u);

        if (control->conn) {
            Controller_run();
        } else {
            puts("Could not connect");
        }
    }
    return 0;
}