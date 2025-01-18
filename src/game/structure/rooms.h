#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "../../utils/chained_list.h"

typedef struct room_s Room;

Room* create_room(int posx, int posy);
void free_room(Room* r);

int get_x(Room* room); int get_y(Room* room);
chained_list* get_entities(Room* room);

void setUp(Room* r, Room* up);
void setDown(Room* r, Room* down);
void setLeft(Room* r, Room* left);
void setRight(Room* r, Room* right);

void clearUp(Room* r, Room* up);
void clearDown(Room* r, Room* down);
void clearLeft(Room* r, Room* left);
void clearRight(Room* r, Room* right);