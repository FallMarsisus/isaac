#pragma once

#include <stdbool.h>

typedef struct {
    int damage; 
    int knockback;
    bool is_poisonnous;
    int delay;
} DamagerComponent;