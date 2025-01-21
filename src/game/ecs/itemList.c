#include "itemList.h"

int itemCount = 5;
const int max_items = 5;


ItemComponent potion = {POTION, "Potion", "A healing potion", 25};
ItemComponent apple = {APPLE, "Apple", "A delicious apple", 10};
ItemComponent sword = {SWORD, "Sword", "A sharp sword", 50};
ItemComponent key = {KEY, "Key", "A key to open doors", 5};
ItemComponent shield = {SHIELD, "Shield", "A shield to protect you", 20};

ItemComponent* itemList[] = {&potion, &apple, &sword, &key, &shield};