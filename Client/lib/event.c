//
// Created by Kevin on 12/10/2015.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "event.h"
#include "connection.h"

/**
 * Process the data from the server to run specific events.
 *
 * @param data the data received from the server
 * @param len the length of the data.
 */
void Event_run(char **data, int len) {
    char *key = data[0];

    if (strcmp(key, "login") == 0) { //login response
        _event_login(atoi(data[1]), data[2]);
    } else if (strcmp(key, "newgame") == 0) { //newgame response
        _event_new_game(atoi(data[1]), atoi(data[2]), atoi(data[3]));
    } else if (strcmp(key, "guess") == 0) { //guess reosonse
        _event_guess(atoi(data[1]), atoi(data[2]), data[3], data[4], data[5]);
    } else if (strcmp(key, "leaderboard") == 0) { //leaderboard data
        _event_leaderboard(data);
    } else if (strcmp(key, "connect") == 0) { //connection response
        _event_connect(atoi(data[1]), data[2]);
    }
}

/**
 * Event for when the user connects to the server.
 * Handles whether the connection was successful or not.
 *
 * @param success whether the connection was successful or not
 * @param msg the message associated with the response
 */
void _event_connect(int success, char *msg) {
    if (success == 0) { //if the login was successful
        puts(msg);
        puts("Exitting the program...");
        Control_exit();
    }
    stop_waiting();
}

/**
 * Converts the data into a Leaderboard instance, and assigns
 * to the control instance.
 *
 * @param data the data from the server
 */
void _event_leaderboard(char **data) {
    int count = atoi(data[1]), i, wins, played, base = 2, z;
    //delete previous leaderboard
    Free_leaderboard(&control->leaderboard);

    //create a new leaderboard instance
    control->leaderboard = newLeaderboard(count);

    //assign each data value to the leaderboard points.
    for (i = 0; i < count; i++) {
        z = base + i * 3;

        control->leaderboard->names[i] = data[z];
        control->leaderboard->played[i] = atoi(data[z + 1]);
        control->leaderboard->wins[i] = atoi(data[z + 2]);
    }

    stop_waiting();
}

/**
 * Passes the guess response back into the game data.
 *
 * @param result what stage of game it is up to.
 * @param nb_left the number of guesses left.
 * @param guesses the string of guesses
 * @param word_a the visible characters in word_a
 * @param word_b the visible characters in word_b
 */
void _event_guess(const int result, const int nb_left, const char *guesses, const char *word_a, const char *word_b) {
    control->game->nb_left = nb_left;
    strcpy(control->game->guesses, guesses);

    //copy the word_a to words[0]
    int i;
    for (i = 0; i < control->game->word_a; i++) {
        if (word_a[i] != '_') {
            control->game->words[0][i] = word_a[i];
        }
    }

    //copy word_b to game word[1]
    for (i = 0; i < control->game->word_b; i++) {
        if (word_b[i] != '_') {
            control->game->words[1][i] = word_b[i];
        }
    }

    control->game->status = result;
    stop_waiting();
}

/**
 * Creates a new game instance with the given response data.
 *
 * @param word_a the length of word_a
 * @param word_b the length of word_b
 * @param guesses the number of guesses.
 */
void _event_new_game(const int word_a, const int word_b, const int guesses) {
    Free_game(&control->game);
    control->game = newGame(word_a, word_b, guesses);
    stop_waiting();
}

/**
 * Either logs the user in, if it was successful, or exits the game.
 *
 * @param success the success boolean
 * @param msg the error msg.
 */
void _event_login(const int success, char *msg) {
    if (success) {
        control->username = msg;
    } else {
        printf("\n%s\n\n", msg);
        Control_exit();
    }
    stop_waiting();
}

/**
 * Gets the words from a string and splits them based on criteria.
 *
 * @param string the string to split
 * @param count the reference int to assign the length of words to.
 * @param split the string to split on.
 */
char **_get_words(char *string, int *count, char *split) {
    char **words = malloc(0), *word;
    int i = 0, x = 0, y, len = strlen(string), cur_len, has_found, cond, cond_len = strlen(split);

    while (x < len) {
        cur_len = 0;
        has_found = 0;
        word = malloc(0);

        while (x < len) {
            cond = 0;
            for (y = 0; y < cond_len; y++) {
                if (string[x] == split[y]) {
                    cond = 1;
                    break;
                }
            }
            if (!cond) {
                has_found = 1;
                cur_len += 1;

                word = realloc(word, cur_len * sizeof(char));
                word[cur_len - 1] = string[x];
            }
            x++;
            if ((cond || x == len) && has_found == 1) {
                break;
            }
        }
        word = realloc(word, (cur_len + 1) * sizeof(char));
        word[cur_len] = '\0';

        if (has_found) {
            words = realloc(words, (i + 1) * sizeof(char*));
            words[i] = word;
            i++;
        }
    }

    (*count) = i;
    return words;
}