#include "teleporter.h"

void on_teleport(Tile* tile, void* cc) {
    player* p = (player*) cc;

    if(p->can_teleport) {
        move_entity(p->body, -640, 0);
        p->can_teleport = false;
        play_timer(p->teleport_timer);
    }
}

Tile* create_teleporter(int posx, int posy, sprite_list* sprites) {
    Tile* tile = create_interactible_tile(posx, posy, 32, 32, sprites);

    get_interact_core(tile)->on_interact = on_teleport;
    return tile;
}