#include "interactible.h"

Tile* create_interactible_tile(int posx, int posy, int width, int height, sprite_list* sprites) {
    Tile* tile = create_tile(posx, posy, width, height, sprites);
    tile->free_tile_other = free_interactive_tile;
    tile->update = update_interactive_tile;
    tile->draw = draw_interactive_tile;

    tile->collide = false;

    tile->other_variables = malloc(sizeof(interactible_core));
    get_interact_core(tile)->key = SDL_SCANCODE_E;
    get_interact_core(tile)->hovered = false;
    get_interact_core(tile)->on_interact = NULL;
    get_interact_core(tile)->free_other_var = NULL;
    get_interact_core(tile)->other_var = NULL;
    get_interact_core(tile)->core = NULL;
    get_interact_core(tile)->icon_draw_box = malloc(sizeof(SDL_Rect));
    *get_interact_core(tile)->icon_draw_box = (SDL_Rect) {
        posx + width / 2 - 8, posy - 20, 16, 16
    };

    return tile;
}
void load_interactive_textures(Tile* tile, SDL_Renderer* ren) {
    get_interact_core(tile)->core = 
        create_core(ren, tile->sprites->iconE_texture, 16, 16)
    ;
    tile->core = create_core(ren, tile->sprites->cobble_texture, 16, 16);
}
void free_interactive_tile(Tile* tile) {
    if(get_interact_core(tile) != NULL) {
        if(get_interact_core(tile)->free_other_var != NULL) {
            get_interact_core(tile)->free_other_var(get_interact_core(tile)->other_var);
        }
        if(get_interact_core(tile)->core != NULL) {
            free_core(get_interact_core(tile)->core);
        }
    }
}

void update_interactive_tile(Tile* tile, void* cc) {
    player* p = (player*) cc;
    get_interact_core(tile)->hovered = checkCollision(tile->hitbox, p->body->hitbox);

    const Uint8 *state = SDL_GetKeyboardState(NULL);

    if(get_interact_core(tile)->hovered && state[get_interact_core(tile)->key]) {
        if(get_interact_core(tile)->on_interact != NULL) {
            get_interact_core(tile)->on_interact(tile, p);
        }
        get_interact_core(tile)->hovered = false;
    }
}

void draw_interactive_tile(Tile* tile, SDL_Renderer* ren) {
    if (tile->sprites != NULL && tile->core == NULL) {
        load_interactive_textures(tile, ren);
    }
    draw_tile(tile, ren);
    if(tile->core != NULL && get_interact_core(tile)->hovered) {
        SDL_RenderCopy(
            ren, 
            get_interact_core(tile)->core->sprite_sheet, 
            NULL, 
            get_interact_core(tile)->icon_draw_box
        );
    }
}