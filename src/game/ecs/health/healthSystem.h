#pragma once

#include <stdint.h>
#include "../ecs.h"
#include "healthComponent.h"
#include "../../../display.h"

bool init_health_component(HealthComponent* health, int max_health, int max_mana, int shield);
bool damage_bypass_shield(uint32_t entity, int damage);
bool damage(uint32_t entity, int damage);
bool heal(uint32_t entity, int healAmount);
bool addEffect(uint32_t entity, void* effect); // à changer 
bool removeEffect(uint32_t entity, void* effect); //idem
bool addShield(uint32_t entity, int shieldAmount);
bool isDead(uint32_t entity);
bool display_health(uint32_t entity, SDL_Renderer* renderer);

void apply_damage_effect(uint32_t entity);
void restore_sprite_color(uint32_t entity);
void update_health_effect(uint32_t entity);