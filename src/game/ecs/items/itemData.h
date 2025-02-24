#pragma once

#include "../components.h"
#include <stdint.h>
#include <stdbool.h>


typedef struct {
    void* data;

    void (*update)(uint32_t entity, SDL_Rect cam);
} ScriptDeWish;

enum ItemID {
    POTION,
    APPLE,
    SWORD,
    KEY,
    SHIELD
};

typedef struct {
	ScriptDeWish* script;
	float timeBeforeScriptActivation;
} ThrowProperties;

typedef struct { //faudra changer ça pour plutot avoir un type et un identifiant unique 
    enum ItemID id;
    char* name;
    char* description;
    int value;
	ThrowProperties* throwProp;
} ItemData;
