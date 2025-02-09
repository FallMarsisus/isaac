#include "vector.h"

void normalize(Vector* v) {
    float len = sqrt(v->x * v->x + v->y * v->y);
    if(len < 0.0001) return;
    v->x = v->x / len;
    v->y = v->y / len;
}

float vectorSize(Vector* v) {
    return sqrt(v->x * v->x + v->y * v->y);
}

float vectorDistance(Vector *v1, Vector *v2)
{
	return sqrt((v1->x - v2->x)* (v1->x - v2->x) + (v1->y - v2->y) * (v1->y-v2->y));
}

float scalaire(Vector *v1, Vector *v2)
{
	return (v1->x * v2->x) + (v1->y * v2->y);
}
