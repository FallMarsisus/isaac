#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>

#include "../../utils/dyn_arrays.h"

typedef struct room_s Room;

Room* create_room(int posx, int posy);
void free_room(Room* r);

void add_entity(Room* r, u_int32_t* id);

dyn_array* get_entities(Room* r);
int get_x(Room* room); int get_y(Room* room);

void setUp(Room* r, Room* up);
void setDown(Room* r, Room* down);
void setLeft(Room* r, Room* left);
void setRight(Room* r, Room* right);

void clearUp(Room* r, Room* up);
void clearDown(Room* r, Room* down);
void clearLeft(Room* r, Room* left);
void clearRight(Room* r, Room* right);