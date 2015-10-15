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
    g->words[0] = calloc(sizeof(char), (size_t) word_a);
    g->words[1] = calloc(sizeof(char), (size_t) word_b);
    g->nb_left = nb_guesses;
    g->guesses = calloc(sizeof(char), (size_t) (1 + nb_guesses));
    g->word_a = word_a;
    g->word_b = word_b;
    g->status = -1;

    return g;
}

void Free_game(Game **g) {
    if (*g != NULL) {
        free((*g)->words[0]);
        free((*g)->words[1]);
        free((*g)->words);
        free((*g)->guesses);
        free((*g));
        (*g) = NULL;
    }
}

void Game_initialise() {
    Game_welcome();
    while (!control->_connect_receieved) { sleep(1); }
    Game_login();
    int input;
    Game_title();
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

void Game_title() {
    printf("\n\n\nWelcome to the hangman Gaming System\n\n\n");
}

void Game_welcome() {
    puts("===========================================\n\n"
                 "Welcome to the Online Hangman Gaming System\n\n"
                 "===========================================\n\n");
}

void Game_login() {
    char username[100], password[100];
    puts("You are required to logon with your registered Username and Password\n");

    printf("Please enter your username-->");
    scanf("%s", username);

    printf("Please enter your password-->");
    scanf("%s", password);

    Connection_login(username, password);
    while (!control->_login_received) {sleep(1);}
}

void Game_menu() {
    puts("\n\n"
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
    while (!control->_game_setup) {sleep(1);}
    while (control->game->nb_left > 0 && control->game->status == -1) {
        _display_hangman();
        Game_guess(_get_guess());
        _display_line();
        while (control->_game_guessing) {sleep(1);}
    }
    _display_hangman();
    _display_results();
}

void Game_show_leaderboard() {
    control->_wait_leaderboard = 1;
    Connection_leaderboard();
    while (control->_wait_leaderboard) {sleep(1);}
    if (control->leaderboard->count > 0) {
        int i;
        for (i = control->leaderboard->count - 1; i >= 0; i--) {
            _display_score(i);
        }
    } else {
        _empty_leaderboard();
    }
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
    puts("\n-----------------------------------------------");
}

char _get_guess() {
    printf("Enter your guess - ");
    char string[100], c;
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

    printf("%s", str);
}

void Game_guess(const char guess) {
    control->_game_guessing = 1;
    char c[1];
    sprintf(c, "%c", guess);
    Connection_send(_prepare_msg(2, "guess", c));
}

void _display_results() {
    puts("Game Over\n\n");
    if (control->game->status == 1) {
        printf("Well done %s! You won this round of Hangman!\n", control->username);
    } else {
        printf("Bad luck %s! You have run out of guesses. The Hangman got you!\n", control->username);
    }
}

void _display_score(int i) {
    __score_line();
    printf("\n\nPlayer\t- %s\nNumber of games won  - %d\nNumber of games played  - %d\n\n",
        control->leaderboard->names[i],
        control->leaderboard->wins[i],
        control->leaderboard->played[i]
    );
    __score_line();
    printf("\n\n");
}

void __score_line() {
    printf("==============================================================");
}

void _empty_leaderboard() {
    __score_line();
    puts("\n\nThere is no information currently stored in the Leader Board. Try again later\n");
    __score_line();
    puts("\n");
}