//
// Created by Kevin on 10/10/2015.
//

#include <stdio.h>
#include "lib/control.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        puts("Client requires an Ip address followed by a Port number");
    } else {
        control = newControl(argv);
        if (control->conn != NULL) {
            if (Connection_initialise()) {
                Game_initialise();
            } else {
                puts("Failed to start thread");
            }
        }
    }
    return 0;
}