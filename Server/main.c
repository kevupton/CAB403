//
// Created by Kevin on 10/10/2015.
//

#include <stdio.h>
#include <sys/unistd.h>
#include "lib/control.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        puts("Server requires a Port number");
    } else {
        control = newControl(argv);

        if (control->conn) {
            Controller_run();
        } else {
            puts("Could not connect");
        }
    }
    return 0;
}