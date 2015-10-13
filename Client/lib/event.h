//
// Created by Kevin on 12/10/2015.
//

#ifndef SERVER_EVENT_H
#define SERVER_EVENT_H

void Event_run(char **data, int len);
void _event_login(const int success, char *msg);
void _event_new_game(const int word_a, const int word_b, const int guesses);
void _event_guess(const int result, const int nb_left, const char *guesses, const char *word_a, const char *word_b);

char **_get_words(char *string, int *count, char *split);
char *lowercase(const char *str);
int equals(const char *a, const char *b);
#endif //SERVER_EVENT_H
