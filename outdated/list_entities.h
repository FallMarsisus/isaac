#ifndef LIST_ENTITIES_H_
#define LIST_ENTITIES_H_

#include "entities.h"

#include <stdlib.h>
#include <assert.h>

//Listes chaineés avec sentinelle au début prce que c marrant
typedef struct cell_e* list_entity;

list_entity create_list();
void free_entity_list(list_entity l);

int get_len(list_entity l);
entity* get_first(list_entity l);

void append_entity(list_entity l, entity* e);
void remove_entity(list_entity l, entity* e);

void iter(list_entity l, void(*f)(entity*));

#endif