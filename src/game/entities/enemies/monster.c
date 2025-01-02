#include "monster.h"

enemy* create_monster(int x, int y) {
    enemy* e = create_enemy(x, y, 32, 32);
    //load_enemy_textures(e, ren, "assets/monster.png");

    set_update(e, update_monster);
    return e;
}

void update_monster(enemy* e) {
    move_enemy(e, 5, 5);
}