#include "display.h"

sprite_list* sl;

// Load a texture from a file
SDL_Texture* load_texture(const char* path, SDL_Renderer* renderer) {
    SDL_Surface* surface = SDL_LoadBMP(path);
    if (!surface) {
        fprintf(stderr, "Failed to load BMP file: %s\n", SDL_GetError());
        return NULL;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

void load_sprites(SDL_Renderer* ren) {
    sl = malloc(sizeof(sprite_list));
    sl->player_texture = load_texture("assets/entities/player.bmp", ren);
    sl->alien_texture = load_texture("assets/entities/alien.bmp", ren);
    sl->goblin_texture = load_texture("assets/entities/goblin.bmp", ren);
    sl->slime_texture = load_texture("assets/entities/slime.bmp", ren);

    sl->inv_texture = load_texture("assets/inventory/inv_grid.bmp", ren);
    sl->apple_item_texture = load_texture("assets/inventory/apple.bmp", ren);
    sl->unknown_item_texture = load_texture("assets/inventory/unknown_item.bmp", ren);
    sl->sword_item_texture = load_texture("assets/inventory/sword_item.bmp", ren);

    sl->sword_texture = load_texture("assets/sword/spritesheet.bmp", ren);
    sl->sword_up_texture = load_texture("assets/sword/spritesheet_up.bmp", ren);
    sl->sword_down_texture = load_texture("assets/sword/spritesheet_down.bmp", ren);
    sl->sword_mirror_texture = load_texture("assets/sword/spritesheet_mirror.bmp", ren);

    sl->cobble_texture = load_texture("assets/tilemap/cobble.bmp", ren);
    sl->iconE_texture = load_texture("assets/tilemap/iconE.bmp", ren);
    sl->teleporter_texture = load_texture("assets/tilemap/teleporter.bmp", ren);
    sl->chest_opened_texture = load_texture("assets/tilemap/chest_opened.bmp", ren);
    sl->chest_closed_texture = load_texture("assets/tilemap/chest_closed.bmp", ren);
    sl->background_texture = load_texture("assets/tilemap/background.bmp", ren);

    sl->red_heart_full_texture = load_texture("assets/health/red_heart_full.bmp", ren);
    sl->red_heart_half_texture = load_texture("assets/health/red_heart_half.bmp", ren);
    sl->heart_empty_texture = load_texture("assets/health/heart_empty.bmp", ren);
}

sprite_list* get_sprites() {
    return sl;
}