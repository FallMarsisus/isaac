#pragma once

#include <SDL2/SDL.h>

typedef struct {

    SDL_Texture* player_texture;

    SDL_Texture* alien_texture;

    SDL_Texture* goblin_texture;

    SDL_Texture* apple_item_texture;

    SDL_Texture* sword_slash;

    SDL_Texture* cobble_texture;

    SDL_Texture* iconE_texture;

    SDL_Texture* teleporter_texture;

    SDL_Texture* inv_texture;

} sprite_list;
sprite_list* load_sprites(SDL_Renderer* ren);