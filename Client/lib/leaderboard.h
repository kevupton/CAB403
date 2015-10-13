//
// Created by Kevin on 14/10/2015.
//

#ifndef HANGMAN_LEADERBOARD_H
#define HANGMAN_LEADERBOARD_H

typedef struct Leaderboard {
    char **names;
    int *played;
    int *wins;
    int count;
} Leaderboard;


Leaderboard *newLeaderboard(const int count);
void Free_leaderboard(Leaderboard **l);

#endif //HANGMAN_LEADERBOARD_H
