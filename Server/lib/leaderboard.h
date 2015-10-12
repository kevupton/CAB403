//
// Created by Kevin on 12/10/2015.
//

#ifndef SERVER_LEADERBOARD_H
#define SERVER_LEADERBOARD_H

#include "user.h"
#include "userlist.h"

typedef struct Leaderboard {
    char **names;
    int *wins;
    int *played;
    int count;
} Leaderboard;

Leaderboard *newLeaderboard(int start_from, int count);
int _get_score_pos(const int high_pos, const int low_pos, const User *user);
int _get_sub_pos(const User *user, int pos);
void _update_user(User *user);

#endif //SERVER_LEADERBOARD_H
