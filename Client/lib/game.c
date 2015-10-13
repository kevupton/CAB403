//
// Created by Kevin on 10/10/2015.
//

#include <stdio.h>
#include <sys/unistd.h>
#include "game.h"

Game *newGame(const int word_a, const int word_b, const int nb_guesses) {
    Game *g = malloc(sizeof(Game));
    g->words = malloc(2*sizeof(char*));
    g->words[0] = malloc(word_a * sizeof(char));
    g->words[1] = malloc(word_b * sizeof(char));
    g->nb_left = nb_guesses;
    g->guesses = malloc(nb_guesses * sizeof(char));

    return g;
}

void Free_game(Game *g) {
    if (g != NULL) {
        free(g->words[0]);
        free(g->words[1]);
        free(g->words);
        free(g->guesses);
        free(g);
    }
}

void Game_initialise() {
    Game_welcome();
    Game_login();
    int input;

    do {
        Game_menu();
        switch ((input = _menu_input())) {
            case PLAY_HANGMAN:
                Game_play_hangman();
                break;
            case SHOW_LEADERBOARD:
                Game_show_leaderboard();
                break;
        }
    } while (input != QUIT);
}

void Game_board() {

}

void Game_welcome() {
    puts("================================\n\n"
                 "Welcome to the Online Hangman Gaming System\n\n"
                 "================================\n\n");
}

void Game_login() {
    char username[100], password[100];
    puts("You are required to logon with your registered Username and Password\n");

    printf("Please enter your username-- >");
    scanf("%s", username);

    printf("Please enter your password-->");
    scanf("%s", password);

    Connection_login(username, password);

    while (!control->_login_received) {
        sleep(1);
    }
}

void Game_menu() {
    puts("\n\n\nWelcome to the hangman Gaming System\n\n\n\n\n"
                 "Please enter a selection\n"
                 "<1> Play Hangman\n"
                 "<2> Show Leaderboard\n"
                 "<3> Quit\n\n");
}

int _menu_input() {
    int input = 0;
    char in[1];
    do {
        printf("Select option 1-3 ->");
        scanf("%s", in);
        input = atoi(in);
    } while (input < 1 || input > 3);

    return input;
}

void Game_play_hangman() {
    puts("Playing hangman...");
    control->_game_setup = 0;
    Connection_play();
    while (!control->_game_setup) { sleep(1); }
    printf("nb_left = %d\n", control->game->nb_left);
    puts("Hangman");
    sleep(1000);
}

void Game_show_leaderboard() {
    puts("Show Leaderboard");
}
