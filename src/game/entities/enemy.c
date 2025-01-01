#include "enemy.h"

typedef struct enemy_s {
    entity* body;
} enemy;

enemy* create_enemy(int x, int y, int w, int h) {
    enemy* e = malloc(sizeof(enemy));
    e->body = create_entity(x, y, w, h);
    return e;
}
void free_enemy(enemy* e) {
    free_entity(e->body);
    free(e);
}

void update_enemy(enemy* e) {
    update_entity(e->body);
}
void draw_enemy(SDL_Renderer* ren, enemy* e) {
    draw_entity(ren, e->body);
}