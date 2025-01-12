#include "teleporter.h"

void on_teleport(Tile* tile, void* cc) {
    player* p = (player*) cc;

    move_entity(p->body, 0, -100);
}

Tile* create_teleporter(int posx, int posy, sprite_list* sprites) {
    Tile* tile = create_interactible_tile(posx, posy, 32, 32, sprites);

    get_interact_core(tile)->on_interact = on_teleport;
    return tile;
}