//
// Created by Kevin on 12/10/2015.
//

#include <stdlib.h>
#include <stdio.h>
#include "leaderboard.h"
#include "control.h"

Leaderboard *newLeaderboard(int start_from, int count) {
    Leaderboard *l = malloc(sizeof(Leaderboard));
    l->names = malloc(count * sizeof(char*));
    l->wins = malloc(count * sizeof(int));
    l->played = malloc(count * sizeof(int));
    l->count = 0;

    int i;
    for (i = 0; i < count && (i + start_from) < control->userList->count; i++) {
        User *u = control->userList->users[i + start_from];
        if (u->played >= 0) {
            l->names[i] = u->username;
            l->wins[i] = u->won;
            l->played[i] = u->played;
            l->count++;
        }
    }

    return l;
}

//    int pos = _get_score_pos(0, LEADERBOARD_LIMIT - 1, score), i, count = control->leaderboard->count;
//    if (count == 0) {
//        control->leaderboard->scores[0] = score;
//        control->leaderboard->names[0] = board;
//    } else {
//        for (i = count == LEADERBOARD_LIMIT? LEADERBOARD_LIMIT - 1: count; i > pos; i--) {
//            control->leaderboard->scores[i] = control->leaderboard->scores[i - 1];
//            control->leaderboard->names[i] = control->leaderboard->names[i - 1];
//        }
//        control->leaderboard->scores[pos] = score;
//        control->leaderboard->names[pos] = board;
//    }
//
//    if (count < LEADERBOARD_LIMIT)
//        control->leaderboard->count++;
//}

int _get_score_pos(const int high_pos, const int low_pos, const int score) {
    User **u = control->userList->users;
    int half_pos = (low_pos - high_pos)/2 + high_pos,
            low_score  = control->userList->users[low_pos]->won,
            high_score = control->userList->users[high_pos]->won,
            half_score = control->userList->users[half_pos]->won;

    if (score >= high_score) {
        return high_pos;
    } else if (score == low_score) {
        return low_pos;
    } else if (score < low_score) {
        return -1;
    } else {
        if (half_pos == high_pos) return low_pos;
        if (score > half_score) {
            return _get_score_pos(high_pos, half_pos, score);
        } else if (score < half_score) {
            return _get_score_pos(half_pos, low_pos, score);
        } else {
            return half_pos;
        }
    }
}

void _update_user(User *user) {
    int pos = _get_score_pos(0, control->userList->count - 1, user->won);
    printf("pos = %d\n", pos);
    UserList_move_user(user, pos);
}

//void **Leaderboard_get(int start_from, int total) {
//    int i;
//
//    int *scores = malloc(total * sizeof(int));
//    char **names = malloc(total * sizeof(char*));
//    void **arr = malloc(2*sizeof(void*));
//
//    for (i = 0; i < LEADERBOARD_LIMIT && i < total; i++) {
//        scores[i] = control->leaderboard->scores[i + start_from];
//        names[i] = control->leaderboard->names[i + start_from];
//    }
//
//    arr[0] = names;
//    arr[1] = scores;
//
//    return arr;
//}

