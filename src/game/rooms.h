#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "../utils/utils.h"
#include "../utils/chained_list.h"

#include "player.h"

#include "entities/enemies/monster.h"

#include "entities/entities.h"
#include "entities/item.h"
#include "entities/enemy.h"

typedef struct room room;

room* create_room(int posx, int posy);
void free_room(room* r);

int getX(room* r);
int getY(room* r);

void print_room(room* r);

void add_entity(room* r, entity* e);
void add_enemy(room* r, enemy* e);
void add_item(room* r, item* i);

void update_room(player* p, room* r);

void draw_room(SDL_Renderer* ren, room* r);

void setUp(room* r, room* up);
void setDown(room* r, room* down);
void setLeft(room* r, room* left);
void setRight(room* r, room* right);

void clearUp(room* r, room* up);
void clearDown(room* r, room* down);
void clearLeft(room* r, room* left);
void clearRight(room* r, room* right);