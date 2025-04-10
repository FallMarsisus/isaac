#include "../inventory/inventorySystem.h"
#include "./throwScripts.h"
#include "../systems.h"
#include "../../../utils/ouputColors.h"

void free_throw_properties(ThrowProperties* tp) {
	if (tp && tp->script) {

		free(tp->script->data);
		tp->script->data = NULL;
		
		free(tp->script);
		tp->script = NULL;
	}
}

void thrownSwordHit(uint32_t entity, SDL_Rect cam, uint32_t* entites, int amount) {
	//make the sword hit surrounding ennemies and rotate the sprite 
	ItemComponent* item = ECS_GetComponent(entity, ITEM);
	if (!item) return;

	SpriteComponent* sprite = ECS_GetComponent(entity, SPRITE);
	PositionComponent* pos = ECS_GetComponent(entity, POSITION);

	if (!pos) return;

	SDL_Point center = {pos->x + sprite->width/2, pos->y + sprite->height/2};
	float range = sqrt(sprite->width * sprite->width / 4 + sprite->height * sprite->height / 4);

	
	float speed = sqrt(pos->vx * pos->vx + pos->vy * pos->vy);

	if (speed <= 0.001f) {
		speed = 0;
		item->isDropperLocked = false;
		item->isGettable = true;
	} else {
		item->isGettable = false;
	}


	sprite->angle += 8 * speed;


	// add damage to enemies
	for (int i = 0; i < amount; i++) {
		uint32_t target = entites[i];
		if (target == entity || target == item->dropper) continue;

		PositionComponent* targetPos = ECS_GetComponent(target, POSITION);
		SpriteComponent* targetSprite = ECS_GetComponent(target, SPRITE);
		if (!targetPos || !targetSprite) continue;

		SDL_Point targetCenter = {targetSprite->width / 2, targetSprite->height / 2};
		targetCenter.x += targetPos->x;
		targetCenter.y += targetPos->y;

		int dx = targetCenter.x - center.x;
		int dy = targetCenter.y - center.y;
		float distance = sqrt(dx * dx + dy * dy);

		if (distance <= range) {
			// printf(BLUE "Damage: %f, Speed: %f\n" RESET, 3 * speed, speed);
			damage(target, 3*speed);
		}
		
	}
}

void printToDebug(uint32_t entity, SDL_Rect cam, uint32_t* entites, int amount) {
	printf("fonction de con called");
}

void selfDestroyingFunc(uint32_t entity, SDL_Rect cam, uint32_t* entites, int amount) {
	/*
	Pour avoir un modèle (immonde) de fonction qui s'appelle une seule fois quand le timer est fini 
	*/
	ItemComponent* item = ECS_GetComponent(entity, ITEM);
	if (!item) return;

	ScriptComponent* script = ECS_GetComponent(entity, SCRIPT);
	if (!script) return;

	// do stuff

	free(script->data);
	script->update = NULL;
	script->data = NULL;
	
	printf(RED "self destroying func called (and destroyed)\n" RESET);
}

ThrowProperties* get_default_throw_prop(enum ItemID itemType) {
	ThrowProperties* tp = malloc(sizeof(ThrowProperties));
	tp->script = malloc(sizeof(ScriptDeWish));
	tp->script->data = NULL;
	tp->script->update = NULL;
	tp->solidDragCoef = 150;


	switch (itemType) {
	case SWORD: {
		int* dmg = malloc(sizeof(int));
		*dmg = 1;
		tp->script->data = dmg;
		tp->script->update = &thrownSwordHit;
		tp->timeBeforeScriptActivation = 0;
		break;
	}
	
	case APPLE:
		tp->script->data = NULL;
		tp->script->update = &printToDebug;
		tp->timeBeforeScriptActivation = 2;
		break;

	case POTION:
		tp->script->data = NULL;
		tp->script->update = &selfDestroyingFunc;
		tp->timeBeforeScriptActivation = 2;
		break;

	default:
		free_throw_properties(tp);
		free(tp);
		tp = NULL;
		break;
	}

	return tp;
}