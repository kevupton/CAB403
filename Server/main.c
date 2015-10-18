//
// Created by Kevin on 10/10/2015.
//

#include <stdio.h>
#include "lib/control.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        puts("Server requires a Port number");
    } else {
        //create the controller
        control = newControl(argv);

        if (control->conn) { //if  the connection was successful
            Controller_run(); //keep alive
        } else {
            puts("Could not connect");
        }
    }
    return 0;
}