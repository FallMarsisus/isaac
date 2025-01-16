#pragma once

#include <SDL2/SDL.h>
#include <sys/time.h>
#include "../utils/vector.h"
#include "./objects/entities/entity.h"
#include "../utils/chained_list.h"	
#include "../utils/timer.h"

enum weaponTypes {bow, magicWand, sword, axe};


typedef struct {
    int damages;
	int range;
	float damageAngleRad;
	bool weaponWasUsed;
	float cooldownSec;
	Timer* weaponTimer;
    enum weaponTypes weaponType;
	Entity* projectile; //NULL if close range weapon (ex: sword)
} weapon;

weapon* createWeapon(enum weaponTypes type, Entity* projectile);
void attack(weapon* arme, chained_list* entities, Vector* playerPosition, Vector* playerOrientation);
void setWeaponDamages(weapon* arme, int damages);
void setWeaponRange(weapon* arme, int range);
void setWeaponHitAngleRad(weapon* arme, int angleInRad);
void setWeaponHitAngleDegrees(weapon* arme, int angleInDegrees);
void setWeaponCooldown(weapon* arme, float cooldownSec);
