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

    int i = 0;
    User *u;
    for (i = 0; i < count && (i + start_from) < control->users->count; i++) {
        u = control->users->items[i + start_from];
        if (u->played >= 0) {
            l->names[i] = u->username;
            l->wins[i] = u->won;
            l->played[i] = u->played;
            l->count++;
        }
    }

    return l;
}

int _get_score_pos(const int high_pos, const int low_pos, const User *user) {
    int half_pos = (low_pos - high_pos)/2 + high_pos;

    User    *low  = control->users->items[low_pos],
            *high = control->users->items[high_pos],
            *half = control->users->items[half_pos];

    if (high_pos == low_pos) return high_pos;
    if (high == user) {
        high = control->users->items[high_pos + 1];
    }
    if (low == user) {
        low = control->users->items[low_pos - 1];
    }

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
    void **items = control->users->items;
    int x;

    x = pos;
    User *userx = (User *) items[x];
    if (userx->played >= user->played) {
        while (x >= 0 && userx->played >= user->played && userx->won == user->won) {
            pos = x;
            x--;
        }
    } else {
        pos++;
        x = pos;
        while (x < control->users->count && userx->played < user->played && userx->won == user->won) {
            pos = x;
            x++;
        }
    }

    return pos;
}

void _update_user(User *user) {
    int pos = _get_score_pos(0, control->users->count - 1, user);
    List_move(control->users, user, pos);
}

void Free_leaderboard(Leaderboard *l) {
    if (l != NULL) {
        free(l->names);
        free(l->played);
        free(l->wins);
        free(l);
    }
}