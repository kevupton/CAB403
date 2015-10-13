//
// Created by Kevin on 12/10/2015.
//

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "event.h"
#include "connection.h"


void Event_run(char **data, int len) {
    char *key = data[0];

    if (strcmp(key, "login") == 0) {
        _event_login(atoi(data[1]), data[2]);
    } else if (strcmp(key, "newgame") == 0) {
        _event_new_game(atoi(data[1]), atoi(data[2]), atoi(data[3]));
    }
}

void _event_new_game(const int word_a, const int word_b, const int guesses) {
    control->_game_setup = 1;
    Free_game(control->game);
    control->game = newGame(word_a, word_b, guesses);
    puts("Created game");
}

void _event_login(const int success, char *msg) {
    if (success) {
        control->username = msg;
    } else {
        puts(msg);
        Control_exit();
    }
    control->_login_received = 1;
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