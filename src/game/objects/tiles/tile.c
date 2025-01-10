#include "tile.h"

Tile* create_tile(int x, int y, int width, int height, sprite_list* sprites) {
    Tile* tile = malloc(sizeof(Tile));
    tile->hitbox = malloc(sizeof(SDL_Rect));
    tile->hitbox->x = x; tile->hitbox->y = y; tile->hitbox->w = width; tile->hitbox->h = height;

    tile->sprites = sprites;
    return tile;
}
void load_tile_texture(Tile* tile, SDL_Renderer* ren) {
    if(ren == NULL || tile->sprites == NULL || tile->hitbox == NULL) return;
    tile->core = create_core(ren, tile->sprites->player_texture, tile->hitbox->w, tile->hitbox->h);
}
void free_tile(Tile* tile) {
    free_core(tile->core);
    free(tile->hitbox);
    free(tile);
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