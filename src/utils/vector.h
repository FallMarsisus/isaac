#pragma once

#include "math.h"

typedef struct {
    float x;
    float y;
} Vector;

void normalize(Vector* v);
float vectorSize(Vector* v);
float vectorDistance(Vector* v1, Vector* v2);
float scalaire(Vector* v1, Vector* v2);