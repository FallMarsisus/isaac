#pragma once

#include "../components.h"
#include "../ecs.h"

void init_sprite_component(SpriteComponent* sprite, int width, int height, SDL_Texture* tex);
void init_anim_component(AnimationComponent* animation, int sprite_width, int sprite_height);

void free_sprite_component(SpriteComponent* sprite);
void free_anim_component(AnimationComponent* animation);
void free_all_render_components(ECS_Manager* ecs, uint32_t id);

int add_anim(AnimationComponent* animation, float interval, int amount);

void set_active_anim(AnimationComponent* anim, int index);
void play_anim(AnimationComponent* anim);
void pause_anim(AnimationComponent* anim);
void stop_anim(AnimationComponent* anim);

void render_component(uint32_t id, ECS_Manager* ecs, SDL_Rect cam, SDL_Renderer* renderer);