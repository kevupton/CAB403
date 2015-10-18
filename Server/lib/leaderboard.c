//
// Created by Kevin on 12/10/2015.
//

#include <stdlib.h>
#include <stdio.h>
#include "leaderboard.h"


/**
 * Initialises the Leaderboard class
 *
 * @param start_from the nb to start from
 * @param count the nb of scores to take
 *
 * @return Leaderboad* the instance of the Leaderboard
 */
Leaderboard *newLeaderboard(int start_from, int count) {
    //allocate the memory
    Leaderboard *l = malloc(sizeof(Leaderboard));
    l->names = malloc(count * sizeof(char*));
    l->wins = malloc(count * sizeof(int));
    l->played = malloc(count * sizeof(int));
    l->count = 0;

    int i = 0;
    User *u;
    //assign the users to the memory.
    for (i = 0; i < count && (i + start_from) < control->users->count; i++) {
        u = control->users->items[i + start_from];
        if (u->played > 0) { //only if they have played a game
            l->names[l->count] = u->username;
            l->wins[l->count] = u->won;
            l->played[l->count] = u->played;
            l->count++;
        }
    }

    return l;
}

/**
 * Gets the intended position of the selected user with the newly updated score.
 *
 * @param high_pos the location of the highest ranking position i.e. 0
 * @param low_pos the location of the lowest ranking position i.e. count - 1
 * @param user the user to use in the search
 *
 * @return int the new allocated position
 */
int _get_score_pos(const int high_pos, const int low_pos, const User *user) {
    int half_pos = (low_pos - high_pos)/2 + high_pos;

    //get the users of each position
    User    *low  = control->users->items[low_pos],
            *high = control->users->items[high_pos],
            *half = control->users->items[half_pos];

    if (high_pos == low_pos) return high_pos;
    //check if the users are equal in any way, if so then move the pos
    if (high == user) {
        high = control->users->items[high_pos + 1];
    }
    if (low == user) {
        low = control->users->items[low_pos - 1];
    }
    if (half == user && half_pos != high_pos) {
        half = control->users->items[half_pos - 1];
    }

    //work out where it sits using a binary search
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

/**
 * Gets the sub position if the positions end up being equal.
 * Matches on the next level, which is the number of games played.
 *
 * @param user the user to search for.
 * @param pos the position of the search.
 *
 * @return the new int position.
 */
int _get_sub_pos(const User *user, int pos) {
    void **items = control->users->items;
    int x;

    x = pos;
    User *userx = (User *) items[x];

    //check the list going up. until it no longer fits.
    while (x >= 0 && userx->played >= user->played && userx->won == user->won) {
        pos = x;
        x--;
        userx = (User *) items[x];
    }

    x = pos;
    userx = (User *) items[x];
    //check the list going down, until it no longer fits.
    while (x < control->users->count && userx->played < user->played && userx->won == user->won || userx == user) {
        pos = x;
        x++;
        userx = (User *) items[x];
    }

    //return the newly fitted position
    return pos;
}

/**
 * Updates the specified user's position. Should be run whenever the score changes.
 *
 * @param user the user to update
 */
void _update_user(User *user) {
    int pos = _get_score_pos(0, control->users->count - 1, user);
    List_move(control->users, user, pos);
}

/**
 * Frees the memory for the leaderboard.
 *
 * @param l the leaderboard to free the memory of.
 */
void Free_leaderboard(Leaderboard *l) {
    if (l != NULL) {
        free(l->names);
        free(l->played);
        free(l->wins);
        free(l);
    }
}
