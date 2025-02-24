#pragma once

#include "math.h"

typedef struct {
    float x;
    float y;
} Vector;

void normalize(Vector* v);
void vector_multiply(Vector* v, float factor);
float vector_size(Vector* v);
float vector_distance(Vector* v1, Vector* v2);
float scalaire(Vector* v1, Vector* v2);