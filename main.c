//
// Created by Kevin on 10/10/2015.
//

#include <stdio.h>
#include <string.h>
#include "lib/control.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        puts("Client requires an Ip address followed by a Port number");
    } else {
        Control *control = newControl(argv);
        if (control->conn != NULL) {
            puts("Success!");
            printf("Connected to '%s' on port: %d \n", control->conn->ip_address, control->conn->port);

            if (Connection_initialise(control->conn)) {
                char cc[1000];
                while (1) {
                    printf("Enter Character : ");
                    scanf("%s" , cc);
                    puts(cc);

                    Connection_send(control->conn, cc);

                    if (strcmp(cc, "exit") == 0) {
                        break;
                    }
                }
            } else {
                puts("Failed to start thread");
            }

//            Game *g1 = newGame(100);
//
//            GameList_add(control->games, 10, g1);
//            g1 = GameList_get(control->games, 10);
//
//            char string[32];
//            sprintf(string, "%d", g1->id);
//            puts(string);
//
//            puts("\nTEST");
        }
    }
    return 0;
}