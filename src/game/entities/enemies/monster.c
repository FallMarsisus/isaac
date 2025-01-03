#include "monster.h"

enemy* create_monster(int x, int y) {
    enemy* e = create_enemy(x, y, 32, 32);
    //load_enemy_textures(e, ren, "assets/monster.png");

    set_update(e, update_monster);
    return e;
}

void update_monster(player* p, enemy* e) {
    Vector v = {0, 0};
    v.x = get_player_pos(p)->x - get_enemy_pos(e)->x;
    v.y = get_player_pos(p)->y - get_enemy_pos(e)->y;
    if(fabs(v.x) < 0.1 && fabs(v.y) < 0.1) return;
    normalize(&v);
    move_enemy(e, v.x * 2, v.y * 2);
}