#include "healthSystem.h"

bool damage_bypass_shield(ECS_Manager *manager, uint32_t entity, int damage) {
	HealthComponent* health = ECS_GetComponent(manager, entity, HEALTH);

	if (health == NULL || damage < 0) {
		return false;
	}

	health->health -= damage;

	return true;
}

bool damage(ECS_Manager *manager, uint32_t entity, int damage) {
	HealthComponent* health = ECS_GetComponent(manager, entity, HEALTH);

	if (health == NULL || damage < 0) {
		return false;
	}

	if (health->shield - damage < 0) {
		health->shield = 0;
		health->health -= damage - health->shield;
	} else {
		health->shield -= damage;
	}

	return true;
}

bool heal(ECS_Manager *manager, uint32_t entity, int healAmount) {
	HealthComponent* health = ECS_GetComponent(manager, entity, HEALTH);

	if (health == NULL || healAmount < 0) {
		return false;
	}

	health->health += healAmount;
	if (health->health > health->max_health) {
		health->health = health->max_health;
	}

	return true;
}

bool addEffect(ECS_Manager *manager, uint32_t entity, void *effect) {
	// Implementation for adding effect
	return false;
}

bool removeEffect(ECS_Manager *manager, uint32_t entity, void *effect) {
	// Implementation for removing effect
	return false;
}

bool addShield(ECS_Manager *manager, uint32_t entity, int shieldAmount) {
	HealthComponent* health = ECS_GetComponent(manager, entity, HEALTH);

	if (health == NULL || shieldAmount < 0) {
		return false;
	}
	health->shield += shieldAmount;

	return true;
}

bool isDead(ECS_Manager* manager, uint32_t entity) {
	HealthComponent* health = ECS_GetComponent(manager, entity, HEALTH);

	return health != NULL && health->health <= 0; 
}