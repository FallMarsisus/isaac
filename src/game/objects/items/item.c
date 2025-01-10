#include "item.h"

Item* create_item(float x, float y, float w, float h, sprite_list* sprites) {
    Item* item = malloc(sizeof(Item));
    item->pos = malloc(sizeof(Vector)); item->pos->x = x; item->pos->y = y;
    item->hitbox = malloc(sizeof(SDL_Rect)); item->hitbox->x = x; item->hitbox->y = y; item->hitbox->w = w; item->hitbox->h = h;
    item->core = NULL;
    item->sprites = sprites;
    return item;
}
void load_item_textures(Item* item, SDL_Renderer* ren) {
    item->core = create_core(ren, item->sprites->sword_slash, item->hitbox->w, item->hitbox->h);

    add_anim(item->core, 0, 0.1, 2);
    set_active_anim(item->core, 0);
    stop_anim(item->core);
}
void free_item(Item* item) {
    free_core(item->core);
    free(item->hitbox);
    free(item->pos);
    free(item);
}

void draw_item(Item* item, SDL_Renderer* ren) {
    if(item->sprites != NULL && item->core == NULL) {
        load_item_textures(item, ren);
    }

    if(item->core != NULL) {
        draw_core(ren, item->hitbox, item->core);
    }
    else {
        SDL_SetRenderDrawColor(ren, 0, 0, 255, 255);
        SDL_RenderFillRect(ren, item->hitbox);
    }
}