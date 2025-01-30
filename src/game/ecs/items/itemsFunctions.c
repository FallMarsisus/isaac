#include "itemsFunctions.h"

void itemHeal(uint32_t entity, ItemData currentItem) {
	heal(entity, 5); //for the moment heal is not implemented
}

void itemDoNothing(u_int32_t entity, ItemData currentItem) {}