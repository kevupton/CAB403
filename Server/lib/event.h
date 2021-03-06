//
// Created by Kevin on 12/10/2015.
//

#ifndef SERVER_EVENT_H
#define SERVER_EVENT_H

#include "instance.h"

void Event_run(Instance *instance, char **data, int len);
void _event_login(Instance *in, const char *username, const char *password);
void _event_new_game(Instance *in);
void _event_check_guess(Instance *in, char guess);
void _event_leaderboard(Instance *in);

char **_get_words(char *string, int *count, char *split);
char *lowercase(const char *str);
#endif //SERVER_EVENT_H
