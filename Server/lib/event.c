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
    if (strcmp(data[0], "login") == 0) {
        _event_login(data[1], data[2]);
    }
}

void _event_login(const char *username,const char *password) {
    FILE *fp;
    char *line, **words;
    size_t len;
    ssize_t read;
    int nb_words;
    puts("logging in...");

    fp = fopen("Authentication.txt", "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    puts(username);
    puts(password);
    read = getline(&line, &len, fp);
    while ((read = getline(&line, &len, fp)) != -1) {
        words = _get_words(line, &nb_words);

        if (equals(lowercase(words[0]), lowercase(username)) &&
                equals(words[1], password)) {
            //send login success

            return;
        }
    }

    //send login failure

}

char **_get_words(char *string, int *count) {
    char **words = malloc(0), *word;
    int i = 0, x = 0, len = strlen(string), cur_len, has_found;

    while (x < len) {
        cur_len = 0;
        has_found = 0;
        word = malloc(0);

        while (x < len) {
            if (string[x] != ',') {
                has_found = 1;
                cur_len += 1;

                word = realloc(word, cur_len * sizeof(char));
                word[cur_len - 1] = string[x];
            }
            x++;
            if ((string[x] == ',' || x == len) && has_found == 1) {
                break;
            }
        }

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