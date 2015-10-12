//
// Created by Kevin on 12/10/2015.
//

#include <stdlib.h>
#include <stdio.h>
#include "leaderboard.h"

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

int _get_score_pos(const int high_pos, const int low_pos, const User *user) {
    int half_pos = (low_pos - high_pos)/2 + high_pos;

    User    *low  = control->userList->users[low_pos],
            *high = control->userList->users[high_pos],
            *half = control->userList->users[half_pos];

    if (high == user) high = control->userList->users[high_pos + 1];
    if (low == user) low = control->userList->users[low_pos - 1];

    if (user->won > high->won) {
        return high_pos;
    } else if (user->won == high->won) {
        return _get_sub_pos(user, high_pos);
    } if (user->won == low->won) {
        return _get_sub_pos(user, low_pos);
    } else if (user->won < low->won) {
        return -1;
    } else {
        if (half_pos == high_pos) return low_pos;
        if (user->won > half->won) {
            return _get_score_pos(high_pos, half_pos, user);
        } else if (user->won < half->won) {
            return _get_score_pos(half_pos, low_pos, user);
        } else {
            return _get_sub_pos(user, half_pos);
        }
    }
}

int _get_sub_pos(const User *user, int pos) {
    User **users = control->userList->users;
    int x;

    x = pos;

    if (users[x]->played >= user->played) {
        while (x >= 0 && users[x]->played >= user->played && users[x]->won == user->won) {
            pos = x;
            x--;
        }
    } else {
        pos++;
        x = pos;
        while (x < control->userList->count && users[x]->played < user->played && users[x]->won == user->won) {
            pos = x;
            x++;
        }
    }

    return pos;
}

void _update_user(User *user) {
    int pos = _get_score_pos(0, control->userList->count - 1, user);
    printf("pos = %d\n", pos);
    UserList_move_user(user, pos);
}

