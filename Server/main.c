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
    }
    return 0;
}