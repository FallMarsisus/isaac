#ifndef LIST_ROOM_H_
#define LIST_ROOM_H_

#include "rooms.h"

#include <stdlib.h>
#include <assert.h>

//Listes chaineés avec sentinelle au début prce que c marrant
typedef struct cell_r* list_room;

list_room create_entity_list();
void free_entity_list(list_room l);

int get_len(list_room l);
room* get_first(list_room l);

void append_room(list_room l, room* r);
void remove_room(list_room l, room* r);

void iter(list_room l, void(*f)(entity*));

#endif