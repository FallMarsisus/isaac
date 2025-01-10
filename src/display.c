#include "display.h"

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

sprite_list* load_sprites(SDL_Renderer* ren) {
    sprite_list* sl = malloc(sizeof(sprite_list));
    sl->player_texture = load_sprite("assets/player/sprite_sheet.bmp", ren);
    sl->alien_texture = load_sprite("assets/alien/sprite_sheet.bmp", ren);
    sl->sword_slash = load_sprite("assets/player/sword.bmp", ren);
    sl->cobble_texture = load_sprite("assets/tilemap/cobble.bmp", ren);
    sl->iconE_texture = load_sprite("assets/tilemap/iconE.bmp", ren);
    return sl;
}