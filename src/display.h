#pragma once

#include <SDL2/SDL.h>

typedef struct {
    SDL_Texture* player_texture;
    SDL_Texture* alien_texture;
    SDL_Texture* sword_slash;
    SDL_Texture* cobble_texture;
} sprite_list;

sprite_list* load_sprites(SDL_Renderer* ren);