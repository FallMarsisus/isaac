#include "itemList.h"
#include "./throwScripts.h"

const int itemCount = 5;

ItemData potion = {POTION, "Potion", "A healing potion", 25, NULL};
ItemData apple = {APPLE, "Apple", "A delicious apple", 10, NULL};
ItemData sword = {SWORD, "Sword", "A sharp sword", 50, NULL};
ItemData key = {KEY, "Key", "A key to open doors", 5, NULL};
ItemData shield = {SHIELD, "Shield", "A shield to protect you", 20, NULL};

ItemData*itemList[] = {&potion, &apple, &sword, &key, &shield};

void initDefaultItems() {
	for (int i = 0; i < itemCount; i++) {
		itemList[i]->throwProp = get_default_throw_prop(itemList[i]->id);
	}
}