#include "teleporter.h"

void load_teleporter_textures(Tile* tile, SDL_Renderer* ren) {
    tile->core = create_core(ren, tile->sprites->iconE_texture, 16, 16);
    if (tile->core == NULL) {
        fprintf(stderr, "Failed to load enemy texture\n");
        return;
    }
}

void free_teleporter(Tile* tile) {
    free_array(tile->other_variables);
}

void update_teleporter(Tile* tile, void* cc) {
    player* p = (player*) cc;
    *((bool*) get_elt(tile->other_variables, 0)) = checkCollision(tile->hitbox, p->body->hitbox);   
}

void draw_teleporter(Tile* tile, SDL_Renderer* ren) {
    if (tile->sprites != NULL && tile->core == NULL) {
        load_teleporter_textures(tile, ren);
    }
    draw_tile(tile, ren);
    if(tile->core != NULL && *((bool*) get_elt(tile->other_variables, 0))) {
        SDL_RenderCopy(ren, tile->core->sprite_sheet, NULL, get_elt(tile->other_variables, 1));
    }
}

Tile* create_teleporter(int posx, int posy, sprite_list* sprites) {
    Tile* tile = create_tile(posx, posy, 32, 32, sprites);
    tile->free_tile_other = free_teleporter;
    tile->update = update_teleporter;
    tile->draw = draw_teleporter;

    tile->collide = false;

    tile->other_variables = create_array();
    bool* hovered = malloc(sizeof(bool));
    *hovered = false;
    append(tile->other_variables, hovered);

    SDL_Rect* icon_hitbox = malloc(sizeof(SDL_Rect));
    *icon_hitbox = (SDL_Rect) {posx + 8, posy - 20, 16, 16};
    append(tile->other_variables, icon_hitbox);

    return tile;
}

