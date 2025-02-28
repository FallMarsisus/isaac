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
	
	
	// item->isGettable = false;

	// /!\ todo
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


	switch (itemType) {
	case SWORD:
		{int* dmg = malloc(sizeof(int));
		*dmg = 1;
		tp->script->data = dmg;
		tp->script->update = &thrownSwordHit;
		tp->timeBeforeScriptActivation = 1;
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