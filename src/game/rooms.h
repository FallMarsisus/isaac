#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "../utils/utils.h"
#include "../utils/chained_list.h"

#include "objects/entities/player/player.h"
#include "objects/entities/enemies/enemy.h"
#include "objects/entities/entity.h"

#include "objects/items/item.h"

typedef struct room_s {
    int x;
    int y;

    struct room_s* up;
    struct room_s* down;
    struct room_s* left;
    struct room_s* right;

    chained_list* entities;
    chained_list* tiles;
    chained_list* items;
} room;

room* create_room(int posx, int posy);
void add_entity_to_room(room* r, Entity* e);
void add_item_to_room(room* r, Item* item);
void free_room(room* r);

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