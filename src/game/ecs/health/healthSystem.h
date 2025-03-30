#pragma once

#include <stdint.h>
#include "../ecs.h"

#include "../physics/forceSystem.h"
#include "../physics/physicsSystems.h"

#include "healthComponent.h"
#include "../../../display.h"

void init_damager_component(DamagerComponent* damager, int damage, bool damages_player);
bool init_health_component(HealthComponent* health, int max_health);

bool damage(uint32_t entity, uint32_t damager);
bool heal(uint32_t entity, int healAmount);

bool isDead(uint32_t entity);
bool display_health(uint32_t entity, SDL_Renderer* renderer);

void update_health(uint32_t entity);
void draw_health_bar(SDL_Renderer* renderer, uint32_t entity, SDL_Rect cam);