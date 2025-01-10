#include "tile.h"

Tile* create_tile(int x, int y, int width, int height, sprite_list* sprites) {
    Tile* tile = malloc(sizeof(Tile));
    tile->hitbox = malloc(sizeof(SDL_Rect));
    tile->hitbox->x = x; tile->hitbox->y = y; tile->hitbox->w = width; tile->hitbox->h = height;

    tile->sprites = sprites;
    tile->core = NULL;
    tile->collide = true;

    tile->other_variables = NULL;
    tile->free_tile_other = NULL;
    tile->update = NULL;
    tile->draw = NULL;
    return tile;
}
void load_tile_texture(Tile* tile, SDL_Renderer* ren) {
    if(ren == NULL || tile->sprites == NULL || tile->hitbox == NULL) return;
    tile->core = create_core(ren, tile->sprites->cobble_texture, tile->hitbox->w, tile->hitbox->h);
}
void free_tile(Tile* tile) {
    if(tile->free_tile_other != NULL) tile->free_tile_other(tile);
    free_core(tile->core);
    free(tile->hitbox);
    free(tile);
}

void update_tile(Tile* tile, void* pl) {
    if(tile->update != NULL) tile->update(tile, pl);
}

void draw_tile(Tile* tile, SDL_Renderer* ren) {
    if(tile->sprites != NULL && tile->core == NULL) {
        load_tile_texture(tile, ren);
    }

    if(tile->core != NULL) {
        draw_core(ren, tile->hitbox, tile->core);
    }
    else {
        SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
        SDL_RenderFillRect(ren, tile->hitbox);
    }
}