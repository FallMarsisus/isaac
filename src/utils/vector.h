#pragma once

#include "math.h"

typedef struct {
    float x;
    float y;
} Vector;

void normalize(Vector* v);