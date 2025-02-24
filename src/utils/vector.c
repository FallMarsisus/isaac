#include "vector.h"

void normalize(Vector* v) {
    float len = sqrt(v->x * v->x + v->y * v->y);
    if(len < 0.01) return;
    v->x = v->x / len;
    v->y = v->y / len;
}

void vector_multiply(Vector* v, float factor) {
	v->x *= factor;
	v->y *= factor;
}

float vector_size(Vector* v) {
    return sqrt(v->x * v->x + v->y * v->y);
}

float vector_distance(Vector *v1, Vector *v2)
{
	return sqrt((v1->x - v2->x)* (v1->x - v2->x) + (v1->y - v2->y) * (v1->y-v2->y));
}

float scalaire(Vector *v1, Vector *v2)
{
	return (v1->x * v2->x) + (v1->y * v2->y);
}
