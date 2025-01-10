#pragma once

#include "../entity.h"
#include "../../items/item.h"
#include "../../../../utils/dyn_arrays.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct {
    bool* keys; //Keys pressed
    dyn_array* items;

    Entity* body;
} player;

player* create_player(int x, int y, sprite_list* sprites);
void load_player_textures(player* p, SDL_Renderer* ren);
void free_player(player* p);

void add_item_to_player(player* p, Item* item);

void get_inputs(player* p);

void update_player(player* p, chained_list* entities, chained_list* tiles);

void draw_player(SDL_Renderer* ren, player* p);