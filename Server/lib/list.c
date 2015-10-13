//
// Created by Kevin on 10/10/2015.
//

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "list.h"

void List_add(List *list, void *item) {
    if (List_index(list, item) != -1) {
        puts("User already exists");
    } else {
        size_t new_size = list->count + 1;
        void **new_items = realloc(list->items, new_size * list->_type_size);

        if (new_items) {
            list->items = new_items;
            list->items[list->count] = item;
            list->count += 1;
        } else {
            puts("Error users size");
            // deal with realloc failing because memory could not be allocated.
        }
    }
}

int List_index(List *list, void *item) {
    int i = 0;
    for (i; i < list->count; i += 1) {
        if (list->items[i] == item) return i;
    }
    return -1;
}

void *List_get(List *list, int index) {
    return list->items[index];
}

List *newList(int type_size) {
    List *l = malloc(sizeof(List));

    l->items = malloc(0);
    l->count = 0;
    l->_type_size = type_size;

    return l;
}

void List_move(List *list, void *item, int pos) {
    int index = List_index(list, item), len = list->count, i;
    void **items = list->items;

    if (pos != index) {
        if (pos < index) {
            for (i = index; i > pos; i--) {
                items[i] = items[i - 1];
            }
        } else {
            for (i = index; i < pos; i++) {
                items[i] = items[i + 1];
            }
        }
        items[pos] = item;
    }
}
