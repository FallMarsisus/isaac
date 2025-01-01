#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "../utils/utils.h"
#include "../utils/chained_list.h"

#include "entities/entities.h"

typedef struct room room;

room* create_room(int posx, int posy);
void free_room(room* r);

int getX(room* r);
int getY(room* r);

void print_room(room* r);

void add_entity(room* r, entity* e);

void update_room(room* r);

void draw_room(SDL_Renderer* ren, room* r);

void setUp(room* r, room* up);
void setDown(room* r, room* down);
void setLeft(room* r, room* left);
void setRight(room* r, room* right);

void clearUp(room* r, room* up);
void clearDown(room* r, room* down);
void clearLeft(room* r, room* left);
void clearRight(room* r, room* right);