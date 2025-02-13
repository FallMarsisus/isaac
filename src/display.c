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
    sl->player_texture = load_texture("assets/player/sprite_sheet.bmp", ren);
    sl->inv_texture = load_texture("assets/inventory/inv_grid.bmp", ren);
    sl->apple_item_texture = load_texture("assets/inventory/apple.bmp", ren);
    sl->alien_texture = load_texture("assets/alien/sprite_sheet.bmp", ren);
    sl->goblin_texture = load_texture("assets/goblin/sprite_sheet.bmp", ren);
    sl->sword_slash = load_texture("assets/player/sword.bmp", ren);
    sl->cobble_texture = load_texture("assets/tilemap/cobble.bmp", ren);
    sl->iconE_texture = load_texture("assets/tilemap/iconE.bmp", ren);
    sl->teleporter_texture = load_texture("assets/tilemap/teleporter.bmp", ren);
    sl->red_heart_full_texture = load_texture("assets/health/red_heart_full.bmp", ren);
    sl->red_heart_half_texture = load_texture("assets/health/red_heart_half.bmp", ren);
    sl->heart_empty_texture = load_texture("assets/health/heart_empty.bmp", ren);
    sl->chest_opened_texture = load_texture("assets/tilemap/chest_opened.bmp", ren);
    sl->chest_closed_texture = load_texture("assets/tilemap/chest_closed.bmp", ren);
    sl->unknown_item_texture = load_texture("assets/inventory/unknown_item.bmp", ren);
    sl->sword_item_texture = load_texture("assets/inventory/sword_item.bmp", ren);
    sl->background_texture = load_texture("assets/tilemap/background.bmp", ren);

    sl->slime_texture = load_texture("assets/alien/slime.bmp", ren);
    sl->slime_jump_texture = load_texture("assets/alien/slime.bmp", ren);
}

sprite_list* get_sprites() {
    return sl;
}