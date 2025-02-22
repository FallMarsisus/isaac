#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "forceSystem.h"
#include "../components.h"
#include "../ecs.h"

bool wind_force(uint32_t entity, Force* f, void* args) {
	PositionComponent* pos = ECS_GetComponent(entity, POSITION);
	
	if (!pos) return false;
	
	float windVX = ((float*)args)[0];
	float windVY = ((float*)args)[1];
	float windStrength = ((float*)args)[2];
	float deltaVX = windVX - pos->vx;
	float deltaVY = windVY - pos->vy;
	
	f->Fx = windStrength * deltaVX;
	f->Fy = windStrength * deltaVY;
	// printf("deltaVX = %.2f, deltaVY = %.2f\n", deltaVX, deltaVY);
	// printf("applying wind : force = %.2f N\n", sqrt(f->Fx*f->Fx + f->Fy*f->Fy));

	return false;
}

bool solid_drag_force(uint32_t entity, Force* f, void* args) {
	PositionComponent* pos = ECS_GetComponent(entity, POSITION);
	
	if (!pos) return true;

	float coeff = ((float*)args)[0];

	if (pos->vx <= 0.001f && pos->vy <= 0.001f) return true;

	f->Fx = - coeff * pos->vx;
	f->Fy = - coeff * pos->vy;
	return false;
}

Force* create_force(ForceFunction func, void* additionalArgs) {
	Force* f = malloc(sizeof(Force));
	f->func = func;
	f->Fx = 0;
	f->Fy = 0;
	f->additionalArgs = additionalArgs;
	return f;
}

bool update_entity_force(uint32_t entity, Force* f)
{
    return (*f->func)(entity, f, f->additionalArgs);
}

void free_force(Force* f) {
	free(f->additionalArgs);
	free(f);
}
