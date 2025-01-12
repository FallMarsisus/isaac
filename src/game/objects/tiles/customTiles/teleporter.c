#include "teleporter.h"

void on_teleport(Tile* tile, void* cc) {
    player* p = (player*) cc;

    if(p->can_teleport) {
        int dx = ((int*) get_elt(get_interact_core(tile)->other_var, 0))[0];
        int dy = ((int*) get_elt(get_interact_core(tile)->other_var, 0))[1];
        if(dx != 0 || dy != 0) {
            move_entity(p->body, dx, dy);
            printf("%d, %d\n", dx, dy);
            p->can_teleport = false;
            play_timer(p->teleport_timer);
        }
    }
}

void set_other_teleporter(Tile* t1, Tile* t2, int dx, int dy) {
    ((int*) get_elt(get_interact_core(t1)->other_var, 0))[0] = dx;
    ((int*) get_elt(get_interact_core(t1)->other_var, 0))[1] = dy;

    if(get_len(get_interact_core(t1)->other_var) < 2) {
        append(get_interact_core(t1)->other_var, t2);
    }
    else {
        set_elt(get_interact_core(t1)->other_var, 1, t2);
    }
}

Tile* create_teleporter(int posx, int posy, sprite_list* sprites) {
    Tile* tile = create_interactible_tile(posx, posy, 64, 64, sprites);

    get_interact_core(tile)->on_interact = on_teleport;
    get_interact_core(tile)->other_var = create_array();

    int* other_relative_coords = malloc(sizeof(int) * 2);
    other_relative_coords[0] = 0; other_relative_coords[1] = 0;
    append(get_interact_core(tile)->other_var, other_relative_coords);
    return tile;
}
Teleporter_pair* create_pair_teleporter(int x_1, int y_1, int x_2, int y_2, sprite_list* sprites) {
    Teleporter_pair* pair = malloc(sizeof(Teleporter_pair));
    pair->x_1 = x_1; pair->y_1 = y_1;
    pair->x_2 = x_2; pair->y_2 = y_2;

    int t1_x = random_int(100, 540), t1_y = random_int(100, 260);
    int t2_x = random_int(100, 540), t2_y = random_int(100, 260);

    pair->teleporter1 = create_teleporter(t1_x, t1_y, sprites);
    pair->teleporter2 = create_teleporter(t2_x, t2_y, sprites);

    set_other_teleporter(
        pair->teleporter1, 
        pair->teleporter2,
        t2_x - t1_x + 640 * (x_2 - x_1),
        t2_y - t1_y + 360 * (y_2 - y_1)
    );
    set_other_teleporter(
        pair->teleporter2, 
        pair->teleporter1,
        t1_x - t2_x + 640 * (x_1 - x_2),
        t1_y - t2_y + 360 * (y_1 - y_2)
    );

    return pair;
}