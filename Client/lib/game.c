//
// Created by Kevin on 10/10/2015.
//

#include <stdio.h>
#include <sys/unistd.h>
#include <string.h>
#include "game.h"

Game *newGame(const int word_a, const int word_b, const int nb_guesses) {
    Game *g = malloc(sizeof(Game));
    g->words = malloc(2*sizeof(char*));
    g->words[0] = malloc(word_a * sizeof(char));
    g->words[1] = malloc(word_b * sizeof(char));
    g->nb_left = nb_guesses;
    g->guesses = malloc((1 + nb_guesses) * sizeof(char));
    g->word_a = word_a;
    g->word_b = word_b;

    return g;
}

void Free_game(Game *g) {
    if (g != NULL) {
        free(g->words[0]);
        free(g->words[1]);
        free(g->words);
        free(g->guesses);
        free(g);
        g = NULL;
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
    control->_game_setup = 0;
    Connection_play();
    char guess;
    while (!control->_game_setup) { sleep(1); }
    while (control->game->nb_left > 0) {
        _display_hangman();
        guess = _get_guess();
        _display_line();


        sleep(1000);
    }
    sleep(1000);
}

void Game_show_leaderboard() {
    puts("Show Leaderboard");
}

void _display_hangman() {
    printf("\n\nGuessed letters: %s\n\n"
                   "Number of guesses left: %d\n\n"
                   "Word: ",
            control->game->guesses,
            control->game->nb_left);
    _print_word();
    printf("\n\n");

}

void _display_line() {
    puts("\n-----------------------------------------------\n\n");
}

char _get_guess() {
    printf("Enter your guess - ");
    char *string, c;
    int i = 0;
    do {
        scanf("%s", string);
        while ((c = string[i])) {
            if (c != ' ' && c != '\n' && c != '\t')
                return c;
            i++;
        }
    } while (1);
}

void _print_word() {
    int i, x = 0, y = 0, len = 2*(control->game->word_a + control->game->word_b + 1);
    char str[len], *word_a = control->game->words[0], *word_b = control->game->words[1];
    memset(str, ' ', (size_t) (len - 1));

    for (i = 0; i < control->game->word_a; i++) {
        str[x] = word_a[i] != '\0'? word_a[i]: '_';
        x += 2;
    }

    x += 2;
    for (i = 0; i < control->game->word_b; i++) {
        str[x] = word_b[i] != '\0'? word_b[i]: '_';
        x += 2;
    }

    printf(str);
}

void Game_guess(char guess) {
    char c[1];
    sprintf(c, "%c", guess);

    Connection_send(_prepare_msg(2, "guess", c));
}
