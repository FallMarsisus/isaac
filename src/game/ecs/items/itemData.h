#ifndef ITEM_DATA_H
#define ITEM_DATA_H

#include <stdint.h>
#include <stdbool.h>


enum ItemID {
    POTION,
    APPLE,
    SWORD,
    KEY,
    SHIELD
};

typedef struct { //faudra changer ça pour plutot avoir un type et un identifiant unique 
    enum ItemID id;
    char* name;
    char* description;
    int value;
} ItemData;

#endif