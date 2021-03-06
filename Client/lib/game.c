//
// Created by Kevin on 10/10/2015.
//

#include <stdio.h>
#include <string.h>
#include "game.h"

/**
 * Creates a new game instance with the specified inputs.
 *
 * @param word_a the length of the first word
 * @param word_b the length of the second word
 * @param nb_guesses number of guesses that this game has.
 */
Game *newGame(const int word_a, const int word_b, const int nb_guesses) {
    Game *g = malloc(sizeof(Game));

    //allocate the memory
    g->words = malloc(2*sizeof(char*));
    g->words[0] = calloc(sizeof(char), (size_t) word_a + 1);
    g->words[1] = calloc(sizeof(char), (size_t) word_b + 1);

    g->nb_left = nb_guesses;
    g->guesses = calloc(sizeof(char), (size_t) (1 + nb_guesses));

    g->word_a = word_a;
    g->word_b = word_b;

    g->status = -1;

    return g;
}

/**
 * Free the game from memory.
 *
 * @param g the game instance
 */
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

/**
 * Initialises the client side game. This is where all of the
 * game interface logic happens.
 */
void Game_initialise() {
    Game_welcome();
    wait();
    Game_login();
    int input;
    Game_title();
    do { //the game menu - after the login screen
        Game_menu();
        switch ((input = _menu_input())) {
            case PLAY_HANGMAN:
                Game_play_hangman();
                break;
            case SHOW_LEADERBOARD:
                Game_show_leaderboard();
                break;
        }
    } while (input != QUIT); //quit if 3 is pressed
}

/**
 * Print the game title
 */
void Game_title() {
    printf("\n\n\nWelcome to the hangman Gaming System\n\n\n");
}

/**
 * Print the welcome screen
 */
void Game_welcome() {
    puts("===========================================\n\n"
                 "Welcome to the Online Hangman Gaming System\n\n"
                 "===========================================\n\n");
}

/**
 * Perform the game authentication
 */
void Game_login() {
    char username[100], password[100];
    puts("You are required to logon with your registered Username and Password\n");

    printf("Please enter your username-->");
    scanf("%s", username);

    printf("Please enter your password-->");
    scanf("%s", password);

    //do the login on the server side
    Connection_login(username, password);
    wait();
}

/**
 * Displays the game menu
 */
void Game_menu() {
    puts("\n\n"
                 "Please enter a selection\n"
                 "<1> Play Hangman\n"
                 "<2> Show Leaderboard\n"
                 "<3> Quit\n\n");
}

/**
 * Gets the menu input and returns it.
 * Will keep going until the right menu input has been selected.
 *
 * @return int
 */
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

/**
 * Runs the play hangman interface until the game is either lost or won.
 */
void Game_play_hangman() {
    Connection_play();
    wait();

    //while there are still guesses left or game hasnt ended then play the game
    while (control->game->nb_left > 0 && control->game->status == -1) {
        _display_hangman();
        Game_guess(_get_guess()); //send the guess to the server
        _display_line();
        wait();
    }
    _display_hangman();
    _display_results();
}

/**
 * Shows the leaderboard for the game client
 */
void Game_show_leaderboard() {
    Connection_leaderboard();
    wait();
    if (control->leaderboard->count > 0) { //if there are people in the leaderboard
        int i;
        for (i = control->leaderboard->count - 1; i >= 0; i--) { //foreach leaderboard entry
            _display_score(i);
        }
    } else { //else show empty leaderboard
        _empty_leaderboard();
    }
}

/**
 * Displays the hangman text
 */
void _display_hangman() {
    printf("\n\nGuessed letters: %s\n\n"
                   "Number of guesses left: %d\n\n"
                   "Word: ",
            control->game->guesses,
            control->game->nb_left);
    _print_word();
    printf("\n\n");
}

/**
 * Displays the separating line
 */
void _display_line() {
    puts("\n-----------------------------------------------");
}

/**
 * Gets the character from the input
 */
char _get_guess() {
    printf("Enter your guess - ");
    char c;
    scanf(" %c", &c);
    return c;
}

/**
 * prints the hangman words with the spaces in between
 */
void _print_word() {
    int i, x = 0, y = 0, len = 2*(control->game->word_a + control->game->word_b + 1);
    char str[len], *word_a = control->game->words[0], *word_b = control->game->words[1];
    memset(str, ' ', (size_t) len);
    str[len - 1] = '\0';

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

/**
 * Sends the guess to the server
 *
 * @param guess the guess to send to the server
 */
void Game_guess(const char guess) {
    char c[1];
    sprintf(c, "%c", guess);
    Connection_send(_prepare_msg(2, "guess", c));
}

/**
 * Displays the results of the game.
 */
void _display_results() {
    puts("Game Over\n\n");
    if (control->game->status == 1) { //the player won
        printf("Well done %s! You won this round of Hangman!\n", control->username);
    } else { //player lost
        printf("Bad luck %s! You have run out of guesses. The Hangman got you!\n", control->username);
    }
}

/**
 * Displays the leaderboard score of a specific player
 *
 * @param i the index of the players score to display
 */
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

/**
 * Prints the score seperating line
 */
void __score_line() {
    printf("==============================================================");
}

/**
 * Displays the content for if there was an empty leaderboard
 */
void _empty_leaderboard() {
    __score_line();
    puts("\n\nThere is no information currently stored in the Leader Board. Try again later\n");
    __score_line();
    puts("\n");
}