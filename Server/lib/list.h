//
// Created by Kevin on 10/10/2015.
//

#ifndef HANGMAN_LIST_H
#define HANGMAN_LIST_H

#include "game.h"
#include "user.h"

typedef struct Game Game;

typedef struct List {
    int count;
    void **items;
    int _type_size;
} List;

void    List_add(List *list, void *item);
int     List_index(List *list, void *item);
void    List_move(List *list, void *item, int pos);
void   *List_get(List *list, int index);
void    Free_list(List **list);

List *newList(int type_size);

#endif //HANGMAN_LIST_H