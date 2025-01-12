#pragma once

#include "../interactible.h"
#include "../../entities/player/player.h"
#include "../../../../utils/utils.h"
#include "../../../../utils/dyn_arrays.h"

typedef struct {
    int x_1; int y_1;
    Tile* teleporter1;
    int x_2; int y_2;
    Tile* teleporter2;
} Teleporter_pair;

Tile* create_teleporter(int posx, int posy, sprite_list* sprites);

Teleporter_pair* create_pair_teleporter(int x_1, int y_1, int x_2, int y_2, sprite_list* sprites);