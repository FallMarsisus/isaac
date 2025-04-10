#include "healthSystem.h"
#include <limits.h>

void init_damager_component(DamagerComponent* damager, int damage, bool damages_player) {
    damager->damage = damage;
    damager->damage_player = damages_player;
}

bool init_health_component(HealthComponent* health, int max_health) {
	health->health = max_health;
	health->max_health = max_health;

	health->is_invincible = false;
	
	health->last_damage_time = 0;
	health->damage_cooldown = 500;

	return true;
}

bool damage(uint32_t entity, uint32_t damager) {
    HealthComponent* health = ECS_GetComponent(entity, HEALTH);
    if (!health || health->is_invincible) return false;

	PositionComponent* pos1 = ECS_GetComponent(entity, POSITION);
	SpriteComponent* sprite1 = ECS_GetComponent(entity, SPRITE);
	RigidbodyComponent* body = ECS_GetComponent(entity, BODY);
	if(!pos1 || !sprite1 || !body) return false;

	PositionComponent* pos2 = ECS_GetComponent(damager, POSITION);
	SpriteComponent* sprite2 = ECS_GetComponent(entity, SPRITE);
	DamagerComponent* damager_component = ECS_GetComponent(damager, DAMAGER);
	if(!pos2 || !sprite2 || !damager_component) return false;

    Uint32 current_time = SDL_GetTicks();

    health->last_damage_time = current_time;
	health->health -= damager_component->damage;

    if(isDead(entity)) {
        health->health = 0;
        ECS_RemoveEntity(entity);
    }

	health->is_invincible = true;
	sprite1->colored = true;

	float dx = pos1->x - sprite1->width/2 - (pos2->x - sprite2->width / 2);
	float dy = pos1->y - sprite1->height/2 - (pos2->y - sprite2->height/2);
	float len = sqrt(dx*dx + dy*dy);
	if (len > 0) {
		dx /= len;
		dy /= len;
		
		float* knockbackArgs = malloc(sizeof(float) * 3);
		knockbackArgs[0] = dx * 5.0f;  // Vitesse de knockback réduite
		knockbackArgs[1] = dy * 5.0f;
		knockbackArgs[2] = 200.0f;    // Force de knockback augmentée
		
		Force* knockback = create_force(knockback_force, knockbackArgs);
		add_force(entity, knockback);
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

void update_health(uint32_t e) {
	HealthComponent* health = ECS_GetComponent(e, HEALTH);
	SpriteComponent* sprite = ECS_GetComponent(e, SPRITE);
	
	if (health && sprite && sprite->texture) {
		Uint32 current_time = SDL_GetTicks();
		if (current_time > health->last_damage_time + health->damage_cooldown && health->is_invincible) {
			sprite->colored = false;
			health->is_invincible = false;
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
