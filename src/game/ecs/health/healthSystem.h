#pragma once

#include <stdint.h>
#include "../ecs.h"
#include "../healthComponent.h"
#include "../../../display.h"

bool init_health_component(ECS_Manager* manager, uint32_t entity, int max_health, int max_mana, int shield);
bool damage_bypass_shield(ECS_Manager* manager, uint32_t entity, int damage);
bool damage(ECS_Manager* manager, uint32_t entity, int damage);
bool heal(ECS_Manager* manager, uint32_t entity, int healAmount);
bool addEffect(ECS_Manager* manager, uint32_t entity, void* effect); // à changer 
bool removeEffect(ECS_Manager* mananger, uint32_t entity, void* effect); //idem
bool addShield(ECS_Manager* shieldAmout, uint32_t entity, int shieldAmount);
bool isDead(ECS_Manager* manager, uint32_t entity);
bool display_health(ECS_Manager* manager, uint32_t entity, SDL_Renderer* renderer);