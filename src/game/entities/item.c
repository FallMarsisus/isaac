#include "item.h"

typedef struct item_s {
    entity* body;
} item;

item* create_item(int x, int y, int w, int h) {
    item* i = malloc(sizeof(item));
    i->body = create_entity(x, y, w, h);
    return i;
}
void free_item(item* i) {
    free_entity(i->body);
    free(i);
}

void update_item(item* i) {
    update_entity(i->body);
}
void draw_item(SDL_Renderer* ren, item* i) {
    draw_entity(ren, i->body);
}