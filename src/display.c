#include "display.h"

sprite_list* sl;

SDL_Texture* load_sprite(char* path, SDL_Renderer* ren) {
    SDL_Surface* temp = SDL_LoadBMP(path);
    if (temp == NULL) {
        fprintf(stderr, "SDL_LoadBMP Error: %s\n", SDL_GetError());
        return NULL;
    }

    SDL_Texture* sprite_sheet = SDL_CreateTextureFromSurface(ren, temp);
    SDL_FreeSurface(temp);

    if (sprite_sheet == NULL) {
        fprintf(stderr, "SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
        return NULL;
    }

    return sprite_sheet;
}

void load_sprites(SDL_Renderer* ren) {
    sl = malloc(sizeof(sprite_list));
    sl->player_texture = load_sprite("assets/player/sprite_sheet.bmp", ren);
    sl->inv_texture = load_sprite("assets/inventory/inv_grid.bmp", ren);
    sl->apple_item_texture = load_sprite("assets/inventory/apple.bmp", ren);
    sl->alien_texture = load_sprite("assets/alien/sprite_sheet.bmp", ren);
    sl->goblin_texture = load_sprite("assets/goblin/sprite_sheet.bmp", ren);
    sl->sword_slash = load_sprite("assets/player/sword.bmp", ren);
    sl->cobble_texture = load_sprite("assets/tilemap/cobble.bmp", ren);
    sl->iconE_texture = load_sprite("assets/tilemap/iconE.bmp", ren);
    sl->teleporter_texture = load_sprite("assets/tilemap/teleporter.bmp", ren);
}

sprite_list* get_sprites() {
    return sl;
}