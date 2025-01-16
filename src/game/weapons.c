#include "weapons.h"
#include "../utils/pi.h"

void reenableWeapon(void* arme) {((weapon*)arme)->weaponWasUsed = false;}

bool isEnnemyHit(weapon* arme, Entity* entity, Vector* playerPosition, Vector* playerOrientation) {
	float distance = vectorDistance(entity->pos, playerPosition);
	if (distance > arme->range) {return false;}

	Vector toEnemy;
	printf("Entity position: (%f, %f)\n", entity->pos->x, entity->pos->y);
	printf("Player position: (%f, %f) ; orientation: (%f, %f)\n", playerPosition->x, playerPosition->y, playerOrientation->x, playerOrientation->y);
	printf("Distance to enemy: %f\n", distance);
	toEnemy.x = entity->pos->x - playerPosition->x;
	toEnemy.y = entity->pos->y - playerPosition->y;

	if (vectorSize(&toEnemy) != 0) {
		normalize(&toEnemy);
	}

	Vector direction = *playerOrientation;
	if (vectorSize(&direction) != 0) {
		normalize(&direction);
	}

	float produitScalaire = scalaire(&toEnemy, &direction);
	float angleToEnemy = acos(produitScalaire);



	printf("Angle to enemy: %f radians\n", angleToEnemy);
	printf("Maximum damage angle: %f radians\n", (arme->damageAngleRad) / 2.);

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
	arme->damageAngleRad = 2;

	arme->weaponWasUsed = false;
	arme->cooldownSec = 5.;
	arme->weaponTimer = create_timer(reenableWeapon, arme);

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
	printf("Weapon hit angle set to: %f radians\n", arme->damageAngleRad);
}
void setWeaponCooldown(weapon* arme, float cooldownSec) {
	arme->cooldownSec = cooldownSec;
}

void attack(weapon* arme, chained_list* entities, Vector* playerPosition, Vector* playerOrientation) {

	struct timeval current;
	gettimeofday(&current, NULL);

	if (arme->weaponWasUsed) {return;}

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

	play_timer(arme->weaponTimer, arme->cooldownSec);
	arme->weaponWasUsed = true;
}