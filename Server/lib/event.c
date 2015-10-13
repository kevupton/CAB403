//
// Created by Kevin on 12/10/2015.
//

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "event.h"
#include "connection.h"


void Event_run(Instance *instance, char **data, int len) {
    char *key = data[0];
    if (strcmp(key, "login") == 0) {
        _event_login(instance, data[1], data[2]);
    } else if (strcmp(key, "newgame") == 0) {
        _event_new_game(instance);
    } else if (strcmp(key, "guess") == 0) {
        _event_check_guess(instance, data[1][0]);
    }
}

void _event_check_guess(Instance *in, char guess) {
    if (in->game->nb_left > 0) {
        int i;
        char *word_a = in->game->words[0];
        char *word_b = in->game->words[1];
        int word_a_comp = 1, word_b_comp = 1;
        guess = tolower(guess);

        for (i = 0; i < in->game->word_a; i++) {
            if (tolower(word_a[i]) == guess) {
                in->game->visible[0][i] = guess;
            }
            if (in->game->visible[0][i] == '\0')
                word_a_comp = 0;
        }

        for (i = 0; i < in->game->word_b; i++) {
            if (tolower(word_b[i]) == guess) {
                in->game->visible[1][i] = guess;
            }
            if (in->game->visible[1][i] == '\0')
                word_b_comp = 0;
        }

        in->game->guesses[in->game->nb_guesses] = guess;
        in->game->nb_guesses++;
        in->game->nb_left--;

        char *return_nb = "-1";
        if (word_a_comp && word_b_comp) {
            return_nb = "1";
        } else if (in->game->nb_left == 0) {
            return_nb = "0";
        }

        Connection_write(
                in->_sock,
                _prepare_msg(
                    6,
                    "guess",
                    return_nb,
                    in->game->nb_left,
                    in->game->guesses,
                    in->game->visible[0],
                    in->game->visible[1]
                )
        );
    }
}

void _event_new_game(Instance *in) {
    puts("Creating new game\n");
    Free_game(in->game);
    in->game = newGame(&in->prev_game_index);

    char *str = malloc(DATA_LENGTH * sizeof(char));
    sprintf(str, "newgame,%d,%d,%d",
            (int) strlen(in->game->words[0]),
            (int) strlen(in->game->words[1]),
            in->game->nb_left);
    Connection_write(in->_sock, str);
}

void _event_login(Instance *in, const char *username, const char *password) {
    int i;

    void **pair;
    for (i = 0; i < control->auth->count; i++) {
        pair = List_get(control->auth, i);
        if (equals(lowercase(username), lowercase(pair[0])) && equals(password, pair[1])) {
            Connection_write(in->_sock, _prepare_msg(3, "login", "1", pair[0]));
            puts("Login Success");
            return;
        }
    }
    Connection_write(in->_sock, _prepare_msg(3, "login", "0", "You entered either an incorrect username or password - disconnecting"));
    puts("Login Failed");
}

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

char *lowercase(const char *str) {
    int i = 0;
    char *copy = malloc(sizeof(str));

    while( str[i] )
    {
        copy[i] = tolower(str[i]);
        i++;
    }

    return copy;
}


int equals(const char *a, const char *b) {
    int i, len_a = strlen(a), len_b = strlen(b);

    for (i = 0; i < len_a && i < len_b; i++) {
        if (a[i] != b[i]) return 0;
        if (a[i] == '\0' && b[i] == '\0') return 1;
    }

    return 1;
}