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

typedef enum {
    ENEMY,
    ITEM,
    ENTITY_SIMPLE
} entity_type;

typedef struct {
    entity_type type;
    void* data;
} entity_container;

typedef struct room_s{
    int x;
    int y;

    struct room_s* up;
    struct room_s* down;
    struct room_s* left;
    struct room_s* right;

    chained_list* entities;
} room;

room* create_room(int posx, int posy);
void free_room(room* r);

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