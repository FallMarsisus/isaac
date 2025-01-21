#include "itemList.h"

const int itemCount = 5;

ItemData potion = {POTION, "Potion", "A healing potion", 25};
ItemData apple = {APPLE, "Apple", "A delicious apple", 10};
ItemData sword = {SWORD, "Sword", "A sharp sword", 50};
ItemData key = {KEY, "Key", "A key to open doors", 5};
ItemData shield = {SHIELD, "Shield", "A shield to protect you", 20};

ItemData* itemList[] = {&potion, &apple, &sword, &key, &shield};