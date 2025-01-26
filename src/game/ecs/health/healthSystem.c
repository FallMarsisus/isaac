#include "healthSystem.h"

bool init_health_component(uint32_t entity, int max_health, int max_mana, int shield)
{
    HealthComponent* health = ECS_AddComponent(entity, HEALTH, sizeof(HealthComponent));
	health->health = max_health;
	health->mana = 0;
	health->max_health = max_health;
	health->max_mana = max_mana;

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

bool damage(uint32_t entity, int damage) {
	HealthComponent* health = ECS_GetComponent(entity, HEALTH);

	if (health == NULL || damage < 0) {
		return false;
	}

	if (health->shield - damage < 0) {
		health->shield = 0;
		health->health -= damage - health->shield;
	} else {
		health->shield -= damage;
	}


	if(isDead(entity)) {
		health->health = 0;
		exit(0);
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
	int screenWidth;
	SDL_GetRendererOutputSize(renderer, &screenWidth, NULL);

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
