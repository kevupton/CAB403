//
// Created by Kevin on 10/10/2015.
//

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "list.h"

/**
 * Adds an item to a specific list. Dynamically making the array larger.
 *
 * @param list the list to add to.
 * @param item the item to add to the list.
 */
void List_add(List *list, void *item) {
    if (List_index(list, item) != -1) { //if the item exists.
        puts("User already exists");
    } else { //else add the user

        //allocate more memory.
        size_t new_size = (size_t) list->count + 1;
        void **new_items = realloc(list->items, new_size * list->_type_size);

        if (new_items) { //if the memory allocation worked then continue.
            list->items = new_items;
            list->items[list->count] = item;
            list->count += 1;
        } else {
            puts("Error users size");
        }
    }
}

/**
 * Gets the index of a specific item in the list. If the item
 * is not found then returns -1.
 *
 * @param list the list to search
 * @param item the item to search for.
 *
 * @return int the index in the list or -1 if not found.
 */
int List_index(List *list, void *item) {
    int i = 0;
    for (i; i < list->count; i += 1) {
        if (list->items[i] == item) return i;
    }
    return -1;
}

/**
 * Returns an item from the list.
 *
 * @param list the list to get from
 * @param index the index in the list to get.
 *
 * @return void* the item in the list.
 */
void *List_get(List *list, int index) {
    return list->items[index];
}

/**
 * Creates a new list with items of specified size.
 *
 * @param type_size the size of the items in the list.
 *
 * @return List* the newly created List
 */
List *newList(int type_size) {
    List *l = malloc(sizeof(List));

    //define the attributes.
    l->items = malloc(0);
    l->count = 0;
    l->_type_size = type_size;

    return l;
}

/**
 * Moves an item in the list to another spot.
 *
 * @param list the list to use.
 * @param item the item to move.
 * @param pos the index of the new position.
 */
void List_move(List *list, void *item, int pos) {
    int index = List_index(list, item), len = list->count, i;
    void **items = list->items;

    if (pos != index && pos >= 0) {
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

/**
 * Free the data associated to the list
 *
 * @param list the list to free
 */
void Free_list(List **list) {
    if (list != NULL) {
        int i;
        for (i = 0; i < (*list)->count; i++) {
            free((*list)->items[i]);
        }
        free((*list)->items);
        free((*list));
        (*list) = NULL;
    }
}

/**
 * Pops an item of the start of the list. Returning it,
 * and removing it from the list.
 *
 * @param list the list to pop.
 *
 * @return the item at the start.
 */
void *List_pop(List *list) {
    if (list->count > 0) {
        int i;
        size_t new_length = (size_t) list->count - 1;

        void **item = list->items[0];
        for (i = 0; i < new_length; i++) {
            list->items[i] = list->items[i + 1];
        }

        list->count--;
        list->items = realloc(list->items, new_length * list->_type_size);

        return item;
    }
    return NULL;
}