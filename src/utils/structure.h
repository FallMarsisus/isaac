#pragma once

#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct node_s {
    int key;
    int value;
    struct node_s* next;
} Node;

typedef struct int_dict_s {
    int count;
    int capacity;
    Node** array;
} IntDictionary;

typedef struct node_array_s {
    int len;
    int capacity;
    Node** array;
} NodeArray;

typedef struct structure_s {
    IntDictionary* dict;
    NodeArray* array;
} Structure;

Structure* create_structure();
void free_structure(Structure* st);

//Returns the index in the array of the entity
int get_index(Structure* st, int id);

//DONT TOUCH THAT
void resize_dict(Structure* st, int new_capacity);

//Returns the id of the entity
int create_element(Structure* st);
//Returns the index that has been switched
int remove_element(Structure* st, int id);

/*

To iterate : 
for (int i = 0; i < ECS_GetManager()->st->dict->capacity; i++) {
    Node* current = ECS_GetManager()->st->dict->array[i];
    while (current) {
        current = current->next;
    }
}

*/