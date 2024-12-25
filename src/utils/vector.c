#include "vector.h"

void normalize(Vector* v) {
    float len = sqrt(v->x * v->x + v->y * v->y);
    v->x = v->x / len;
    v->y = v->y / len;
}