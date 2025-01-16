#include "weapons.h"
#include "../utils/pi.h"

bool isEnnemyHit(weapon* arme, Entity* entity, Vector* playerPosition, Vector* playerOrientation) {
	float distance = vectorDistance(entity->pos, playerPosition);
	if (distance > arme->range) {return false;}

	Vector toEnemy;
	toEnemy.x = entity->pos->x - playerPosition->x;
	toEnemy.y = entity->pos->y - playerPosition->y;
	normalize(&toEnemy);

	Vector direction = *playerOrientation;
	normalize(&direction);

	float produitScalaire = scalaire(&toEnemy, &direction);
	float angleToEnemy = acos(produitScalaire);
	
	return angleToEnemy <= arme->damageAngleRad/2;
}

void swordAttack(weapon* arme, chained_list* entities, Vector* playerPosition, Vector* playerOrientation) {

	cell* entityCell = get_first(entities);
	while (entityCell != NULL) {
		Entity* entity = get_data(entityCell);

		if (isEnnemyHit(arme, entity, playerPosition, playerOrientation)) {
			if (damageEntity(entity, arme->damages)) {
				remove_elt(entities, entity);
			}
		}
		entityCell = get_next(entityCell);
	}

}

bool isDistanceType(enum weaponTypes type) {
	return type==bow || type==magicWand;
}


weapon* createWeapon(enum weaponTypes type, Entity* projectile) {


	weapon* arme = malloc(sizeof(weapon));
	arme->weaponType = type;

	if (isDistanceType(type)) {
		if (projectile != NULL) {
			fprintf(stderr, "Trying to assigne projectile to close range weapon!\n");
		}

		//todo
		arme->projectile = projectile;	
	} else {
		arme->projectile = NULL;
	}

	arme->damages = 0;
	arme->cooldownSec = 5;
	arme->damageAngleRad = 2;
	gettimeofday(&arme->lastTimeUsed, NULL);

	return arme;
}

void setWeaponDamages(weapon* arme, int damages) {
	arme->damages = damages;
}
void setWeaponRange(weapon* arme, int range) {
	arme->range = range;
}
void setWeaponHitAngleRad(weapon* arme, int angleInRad) {
	arme->damageAngleRad = angleInRad;
}
void setWeaponHitAngleDegrees(weapon* arme, int angleInDegrees) {
	arme->damageAngleRad = angleInDegrees * (PI / 180);
}
void setWeaponCooldown(weapon* arme, float cooldownSec) {
	arme->cooldownSec = cooldownSec;
}

void attack(weapon* arme, chained_list* entities, Vector* playerPosition, Vector* playerOrientation) {

	struct timeval current;
	gettimeofday(&current, NULL);

	if (current.tv_sec - arme->lastTimeUsed.tv_sec < arme->cooldownSec) {
		return;
	}

	switch (arme->weaponType)
	{
	case bow:
		/* code */
		break;

	case magicWand:
		break;

	case sword:
		swordAttack(arme, entities, playerPosition, playerOrientation);
		break;

	case axe :
	
	default:
		break;
	}
}