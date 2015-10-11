//
// Created by Kevin on 10/10/2015.
//

#include <stdio.h>
#include <string.h>
#include "game.h"

Game *newGame(int id) {
    Game *g = malloc(sizeof(Game));
    g->id = id;
    return g;
}

void Game_initialise() {
    Game_welcome();
    if (Game_login()) {
        char cc[1000];
        while (1) {
            printf("Enter Character : ");
            scanf("%s" , cc);
            puts(cc);

            Connection_send(cc);

            if (strcmp(cc, "exit") == 0) {
                break;
            }
        }
    }
}

void Game_board() {

}

void Game_welcome() {
    puts("================================\n\n"
                 "Welcome to the Online Hangman Gaming System\n\n"
                 "================================\n\n");
}

int Game_login() {
    char username[100], password[100];
    puts("You are required to logon with your registered Username and Password\n");

    printf("Please enter your username-- >");
    scanf("%s", username);

    printf("Please enter your password-->");
    scanf("%s", password);

    if (_valid_login(username, password)) {
        return 1;
    } else {
        puts("You entered either an incorrect username or password - dosconnecting");
        Connection_close();
        return 0;
    }
}

int _valid_login(char *username, char *password) {
    Connection_login(username, password);
}