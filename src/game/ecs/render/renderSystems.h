#pragma once

#include "../components.h"
#include "../ecs.h"

void init_sprite_component(SpriteComponent* sprite, int width, int height, SDL_Texture* tex);
void init_anim_component(AnimationComponent* animation, int sprite_width, int sprite_height);
void init_tile_component(TileComponent* tile, int tile_x, int tile_y, int tile_width, int tile_height);

void free_sprite_component(SpriteComponent* sprite);
void free_anim_component(AnimationComponent* animation);
void free_all_render_components(uint32_t id);

int add_anim(AnimationComponent* animation, float interval, int amount);
int add_anim_tile(AnimationComponent* animation, int start_x, int start_y, float interval, int amount);

void set_active_anim(AnimationComponent* anim, int index);
void play_anim(AnimationComponent* anim);
void pause_anim(AnimationComponent* anim);
void stop_anim(AnimationComponent* anim);

void update_anim(uint32_t id);

/**
 * @brief Renders the background texture to the screen.
 *
 * This function takes a camera rectangle, an SDL renderer, and an SDL texture
 * representing the background, and renders the background texture to the screen
 * using the provided renderer.
 *
 * @param cam The SDL_Rect representing the camera view.
 * @param renderer The SDL_Renderer used to render the texture.
 * @param background The SDL_Texture representing the background to be rendered.
 */
void render_background(SDL_Rect cam, SDL_Renderer *renderer, SDL_Texture *background);

void render_component(uint32_t id, SDL_Rect cam, SDL_Renderer* renderer);