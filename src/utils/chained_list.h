#ifndef CHAINED_LIST_H_
#define CHAINED_LIST_H_

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

typedef enum {
    INT,
    FLOAT,
    STRING
} Datatype;

typedef struct chained_list_s chained_list;

chained_list* create_list();
void free_list(chained_list* l);

void append_elt(chained_list* l, void* element);
void remove_elt(chained_list* l, void* element);

void iter(chained_list* l, void (*f)(void*));

#endif