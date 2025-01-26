#ifndef CHAINED_LIST_H_
#define CHAINED_LIST_H_

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

typedef struct cell_s cell;

typedef struct chained_list_s chained_list;

chained_list* create_list();
void free_list(chained_list* l, bool freeElts);

void append_elt(chained_list* l, void* element);
void remove_elt(chained_list* l, void* element);

void iter(chained_list* l, void (*f)(void*));

cell* get_first(chained_list* l);
cell* get_next(cell* c);
void* get_data(cell* c);

#endif