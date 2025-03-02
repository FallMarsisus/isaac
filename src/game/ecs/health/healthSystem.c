#include "healthSystem.h"

bool init_health_component(HealthComponent* health, int max_health, int max_mana, int shield) {
	health->health = max_health;
	health->mana = 0;
	health->max_health = max_health;
	health->max_mana = max_mana;
	health->isDisplayed = false;
	health->shield = 0;
	health->last_damage_time = 0;
	health->damage_cooldown = 1000; // 1 seconde de cooldown

	return true;
}

bool damage_bypass_shield(uint32_t entity, int damage)
{
    HealthComponent* health = ECS_GetComponent(entity, HEALTH);

	if (health == NULL || damage < 0) {
		return false;
	}

	health->health -= damage;

	return true;
}

void apply_damage_effect(uint32_t entity) {
    SpriteComponent* sprite = ECS_GetComponent(entity, SPRITE);
    HealthComponent* health = ECS_GetComponent(entity, HEALTH);
    
    if (!sprite || !sprite->texture || !health) return;
    
    SDL_SetTextureColorMod(sprite->texture, 255, 50, 50);
    health->effect_end_time = SDL_GetTicks() + 200; // Effet dure 200ms
}

void restore_sprite_color(uint32_t entity) {
    SpriteComponent* sprite = ECS_GetComponent(entity, SPRITE);
    if (sprite && sprite->texture) {
        SDL_SetTextureColorMod(sprite->texture, 255, 255, 255);
    }
}

bool damage(uint32_t entity, int damage) {
    HealthComponent* health = ECS_GetComponent(entity, HEALTH);
    if (!health || damage < 0) return false;

    Uint32 current_time = SDL_GetTicks();
    if (current_time - health->last_damage_time < health->damage_cooldown) {
        return false;
    }

    // Appliquer les dégâts
    if (health->shield - damage < 0) {
        health->shield = 0;
        health->health -= damage - health->shield;
    } else {
        health->shield -= damage;
    }

    apply_damage_effect(entity);
    health->last_damage_time = current_time;

    if(isDead(entity)) {
        health->health = 0;
        ECS_RemoveEntity(entity);
    }

    return true;
}

bool heal(uint32_t entity, int healAmount) {
	HealthComponent* health = ECS_GetComponent(entity, HEALTH);

	if (health == NULL || healAmount < 0) {
		return false;
	}

	health->health += healAmount;
	if (health->health > health->max_health) {
		health->health = health->max_health;
	}


	return true;
}

bool addEffect(uint32_t entity, void *effect) {
	// Implementation for adding effect
	return false;
}

bool removeEffect(uint32_t entity, void *effect) {
	// Implementation for removing effect
	return false;
}

bool addShield(uint32_t entity, int shieldAmount) {
	HealthComponent* health = ECS_GetComponent(entity, HEALTH);

	if (health == NULL || shieldAmount < 0) {
		return false;
	}
	health->shield += shieldAmount;

	return true;
}

bool isDead(uint32_t entity) {
	HealthComponent* health = ECS_GetComponent(entity, HEALTH);

	return health != NULL && health->health <= 0; 
}

bool display_health(uint32_t entity, SDL_Renderer *renderer)
{
	HealthComponent* health = ECS_GetComponent(entity, HEALTH);
	if(!health) return false;

	// Check for death
	if (health->health <= 0) {
		health->health = health->max_health;  // Reset health
		return false;
	}

	int screenWidth;
	SDL_RenderGetLogicalSize(renderer, &screenWidth, NULL);

	for (int i = (health->max_health / 2 + health->max_health % 2) - 1; i >= 0; i--) {
		SDL_Rect heartRect = { screenWidth - 50 - ((health->max_health / 2 + health->max_health % 2) - 1 - i) * 50, 10, 40, 40 }; // Position and size of each heart
		SDL_Texture* heartTexture;

		if (i < health->health / 2) {
			heartTexture = get_sprites()->red_heart_full_texture;
		} else if (i == health->health / 2 && health->health % 2 != 0) {
			heartTexture = get_sprites()->red_heart_half_texture;
		} else {
			heartTexture = get_sprites()->heart_empty_texture;
		}

		SDL_RenderCopy(renderer, heartTexture, NULL, &heartRect);
	}
	return true;
}

void update_health_effect(uint32_t e) {
	HealthComponent* health = ECS_GetComponent(e, HEALTH);
	SpriteComponent* sprite = ECS_GetComponent(e, SPRITE);
	
	if (health && sprite && sprite->texture) {
		Uint32 current_time = SDL_GetTicks();
		if (current_time >= health->effect_end_time) {
			SDL_SetTextureColorMod(sprite->texture, 255, 255, 255);
		}
	}
}

void draw_health_bar(SDL_Renderer* renderer, uint32_t entity, SDL_Rect cam) {
    HealthComponent* health = ECS_GetComponent(entity, HEALTH);
    PositionComponent* pos = ECS_GetComponent(entity, POSITION);
    SpriteComponent* sprite = ECS_GetComponent(entity, SPRITE);
    
    
    // Ne pas afficher la barre de vie du joueur
    if (!health || !pos || !sprite ) return;
    
    // Dimensions de la barre de vie
    int barWidth = sprite->width;
    int barHeight = 6;
    
    // Position de la barre (au-dessus de l'entité)
    int barX = pos->x - cam.x;
    int barY = pos->y - cam.y - 10;
    
    // Dessiner le fond de la barre (rouge)
    SDL_Rect bgRect = {barX, barY, barWidth, barHeight};
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &bgRect);
    
    // Dessiner la barre de vie (vert)
    float healthRatio = (float)health->health / health->max_health;
    SDL_Rect healthRect = {barX, barY, barWidth * healthRatio, barHeight};
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &healthRect);
}
