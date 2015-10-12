//
// Created by Kevin on 12/10/2015.
//

#ifndef SERVER_LEADERBOARD_H
#define SERVER_LEADERBOARD_H

static const int LEADERBOARD_LIMIT;

typedef struct Leaderboard {
    char **names;
    int *scores;
    int *wins;
    int *games;
    int count;
} Leaderboard;

Leaderboard *newLeaderboard();
void Leaderboard_add(char *name, int score);
int _get_score_pos(const int high_pos, const int low_pos, const int score);
void **Leaderboard_get(int start_from, int total);
int _get_user_score(char *username);

#endif //SERVER_LEADERBOARD_H
