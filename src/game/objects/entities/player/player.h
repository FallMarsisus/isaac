#pragma once

#include "../entity.h"
#include "../../../../utils/dyn_arrays.h"
#include "../../../inventory.h"
#include "../../../../utils/timer.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct {
    bool* keys; //Keys pressed

    bool can_teleport;
    Timer* teleport_timer;

    bool is_dashing;
    bool can_dash;
    bool open_inv;

    inv* inv;
    Timer* dash_timer;
    Timer* dash_cooldown;

    bool running;
    Entity* body;
} player;

player* create_player(int x, int y, sprite_list* sprites);
void load_player_textures(player* p, SDL_Renderer* ren);
void free_player(player* p);

void get_inputs(player* p);

void update_player(player* p, chained_list* entities, chained_list* tiles);

void draw_player(SDL_Renderer* ren, player* p);