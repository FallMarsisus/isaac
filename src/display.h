#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct {
    SDL_Texture* player_texture;
    SDL_Texture* alien_texture;
    SDL_Texture* goblin_texture;
    SDL_Texture* slime_texture;
    SDL_Texture* flame_texture;
    SDL_Texture* boss_texture;

    SDL_Texture* inv_texture;
    SDL_Texture* apple_item_texture;
    SDL_Texture* unknown_item_texture;
    SDL_Texture* sword_item_texture;
	SDL_Texture* sword_slot_texture;
	SDL_Texture* armor_slot_texture;
	
    SDL_Texture* sword_texture;
	
    SDL_Texture* teleporter_texture;
    SDL_Texture* background_texture;
    SDL_Texture* tileset_texture;
	
    SDL_Texture* red_heart_full_texture;
    SDL_Texture* red_heart_half_texture;
    SDL_Texture* heart_empty_texture;
} sprite_list;

void init_renderer(SDL_Window* win);

void load_sprites();

sprite_list* get_sprites();
SDL_Renderer* get_renderer();