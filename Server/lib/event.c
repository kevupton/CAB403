//
// Created by Kevin on 12/10/2015.
//

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "event.h"

/**
 * Method for handling the incoming messages.
 * Takes the first param of the message and runs the given method with
 * the received data.
 *
 * @param instance the thread instance
 * @param data the received data from the message
 * @param len the length of the data
 */
void Event_run(Instance *instance, char **data, int len) {
    char *key = data[0];
    if (strcmp(key, "login") == 0) { //for authentication attempts
        _event_login(instance, data[1], data[2]);
    } else if (strcmp(key, "newgame") == 0) { //for new game requests
        _event_new_game(instance);
    } else if (strcmp(key, "guess") == 0) { //for guess attempts
        _event_check_guess(instance, data[1][0]);
    } else if (strcmp(key, "leaderboard") == 0) { //for requesting leaderboard.
        _event_leaderboard(instance);
    }
}

/**
 * Gets the current leaderboard information and sends it to the
 * specified client associated with the instance.
 *
 * @param in the instance containing the connection.
 */
void _event_leaderboard(Instance *in) {
    int start_from = 0, i = 0, x;
    Leaderboard *l = newLeaderboard(start_from, LEADERBOARD_SHOW); //gets the current leaderboard data

    int data_len = l->count * 3;
    char count[5];
    char *data[data_len], *played, *won;

    sprintf(count, "%d", l->count);

    //assign the leaderboard data to a string
    for (i = 0; i < l->count; i++) {
        played = malloc(5 * sizeof(char));
        won = malloc(5 * sizeof(char));

        sprintf(won, "%d", l->wins[i]);
        sprintf(played, "%d", l->played[i]);

        data[i * 3] = l->names[i];
        data[i * 3 + 1] = played;
        data[i * 3 + 2] = won;
    }

    //send the leaderboard data
    Connection_write(in->_sock, _prepare_msg(3, "leaderboard", count, _concat_array(data_len, data, ",")));

    //free the string associated with sending the data.
    for (i = 0; i < l->count; i++) {
        free(data[i * 3 + 1]);
        free(data[i * 3 + 2]);
    }
    Free_leaderboard(l);
}

/**
 * Check the guess with the specified instance.
 * Sends the response data if of the guess, if the guess was allowed.
 *
 * @param in the associated instance
 * @param guess the character of the guess.
 */
void _event_check_guess(Instance *in, char guess) {
    if (in->game != NULL && in->game->nb_left > 0) { //if the guess is allowed
        int i;
        char *word_a = in->game->words[0];
        char *word_b = in->game->words[1];
        int word_a_comp = 1, word_b_comp = 1;
        guess = tolower(guess);

        //check all of the first word characters.
        //assigning the right characters to the visible word
        for (i = 0; i < in->game->word_a; i++) {
            if (tolower(word_a[i]) == guess) {
                in->game->visible[0][i] = guess;
            }
            if (in->game->visible[0][i] == '_')
                word_a_comp = 0;
        }

        //check all of the first word characters.
        //assigning the right characters to the visible word
        for (i = 0; i < in->game->word_b; i++) {
            if (tolower(word_b[i]) == guess) {
                in->game->visible[1][i] = guess;
            }
            if (in->game->visible[1][i] == '_')
                word_b_comp = 0;
        }

        //append the guess to the list of guesses and increment
        in->game->guesses[in->game->nb_guesses] = guess;
        in->game->nb_guesses++;
        in->game->nb_left--;

        //define the current stage of the game.
        char *return_nb = "-1";
        if (word_a_comp && word_b_comp) { //game is won
            return_nb = "1";
        } else if (in->game->nb_left == 0) { //game is lost
            return_nb = "0";
        }

        //game end check.
        int status = atoi(return_nb);
        if (status != -1) { //if the game is over.
            Leaderboard_result(in->user, &status); //add to the leaderboard.
        }

        //send the data back to the client in a readable format.
        char nb_left[10];
        sprintf(nb_left, "%d", in->game->nb_left);
        Connection_write(
                in->_sock,
                _prepare_msg(
                    6,
                    "guess",
                    return_nb,
                    nb_left,
                    in->game->guesses,
                    in->game->visible[0],
                    in->game->visible[1]
                )
        );
    }
}


/**
 * Create a new game for the specified instance.
 *
 * @param in the specified instance.
 */
void _event_new_game(Instance *in) {
    Free_game(&in->game); //free the previous game.
    in->game = newGame(&in->prev_game_index); //make a new game that wasn't the same as the last

    //send the data back to the client, about the length of words and nb of guesses.
    char *str = malloc(DATA_LENGTH * sizeof(char));
    sprintf(str, "newgame,%d,%d,%d",
            (int) strlen(in->game->words[0]),
            (int) strlen(in->game->words[1]),
            in->game->nb_left);
    Connection_write(in->_sock, str);
}

/**
 * Attempts to log the user in, depending on whether or not
 * their username password combination is correct.
 *
 * @param username the string username
 * @param password the string password
 */
void _event_login(Instance *in, const char *username, const char *password) {
    int i;
    char *lower_a = lowercase(username), *lower_b;

    void **pair;
    for (i = 0; i < control->auth->count; i++) { //for each of the loaded authentication data
        pair = List_get(control->auth, i);
        //convert both usernames to lowercase
        lower_b = lowercase(pair[0]);
        if (strcmp(lower_a, lower_b) == 0
            && strcmp(password, pair[1]) == 0) { //if the usernames and passwords match

            //log the user in and send a success message
            in->user = User_login(pair[0]);
            Connection_write(in->_sock, _prepare_msg(3, "login", "1", pair[0]));

            //free the lowercase data
            free(lower_a);
            free(lower_b);
            return;
        }
        //free the lowercase data
        free(lower_b);
    }
    free(lower_a); //^^
    //send a fail response
    Connection_write(in->_sock, _prepare_msg(3, "login", "0", "You entered either an incorrect username or password - disconnecting"));
}

/**
 * Splits a string into an array of words around the given split string.
 *
 * @param string the string to split up
 * @param count reference to the int to give the count to.
 * @param split the split string to split on.
 *
 * @return char** the array of words split up
 */
char **_get_words(char *string, int *count, char *split) {
    char **words = malloc(0), *word;
    int i = 0, x = 0, y, len = strlen(string), cur_len, has_found, cond, cond_len = strlen(split);

    //algorithm to split the string up
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

/**
 * Changes the word to a lowercase word.
 * But doesn't override the existing word.
 *
 * @param str the string to make lowercase
 */
char *lowercase(const char *str) {
    int i = 0;
    size_t size = sizeof(str);
    char *copy = malloc(size);

    while( str[i] )
    {
        copy[i] = tolower(str[i]);
        i++;
    }
    if (i >= size) {
        realloc(copy, size + 1);
    }
    copy[i] = '\0';

    return copy;
}