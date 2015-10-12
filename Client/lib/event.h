//
// Created by Kevin on 12/10/2015.
//

#ifndef SERVER_EVENT_H
#define SERVER_EVENT_H

void Event_run(char **data, int len);
void _event_login(const char *username, const char *password);
char **_get_words(char *string, int *count);
char *lowercase(const char *str);
int equals(const char *a, const char *b);
#endif //SERVER_EVENT_H
