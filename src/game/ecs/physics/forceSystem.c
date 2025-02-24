#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "forceSystem.h"
#include "../components.h"
#include "../ecs.h"
#include "../../../utils/ouputColors.h"

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

bool fluid_drag_force(uint32_t entity, Force* f, void* args) {
	PositionComponent* pos = ECS_GetComponent(entity, POSITION);
	
	if (!pos) return true;

	float coeff = *(float*)args;

	if (fabs(pos->vx) <= 0.005f && fabs(pos->vy) <= 0.005f) return true;

	float norm = sqrt(pos->vx * pos->vx + pos->vy * pos->vy);
	f->Fx = - coeff * pos->vx * norm;
	f->Fy = - coeff * pos->vy * norm;
	printf("\nnorm = %.5f, pos->vx = %.5f, pos->vy = %.5f, coeff = %.5f\n", norm, pos->vx, pos->vy, coeff);
	printf(BLUE "applying solid drag: force = %.5f N\n" RESET, sqrt(f->Fx * f->Fx + f->Fy * f->Fy));
	return false;
}

void stopLeMassacre(PositionComponent* pos, RigidbodyComponent* body, Force* f, bool X, bool Y) {
	if (X) {
		f->Fx = 0;
		pos->vx = 0;
		pos->ax = 0;
		body->forceX = 0;
	} 
	if (Y) {
		f->Fy = 0;
		pos->vy = 0;
		pos->ay = 0;
		body->forceY = 0;
	}
}

bool solid_drag_force(uint32_t entity, Force* f, void* args) {
	PositionComponent* pos = ECS_GetComponent(entity, POSITION);
	RigidbodyComponent* body = ECS_GetComponent(entity, BODY);
	
	if (!pos || !body) return true;

	int seuil = 5;

	float movingCoeff = *(float*)args;
	float staticCoef = ((float*)args)[1];

	// to prevent oscilations
	float lastSignX = ((float*)args)[2]; 
	float timesOsciliatingX = ((float*)args)[3];
	float lastSignY = ((float*)args)[4];
	float timesOsciliatingY = ((float*)args)[5];

	float allForces = sqrt(body->forceX * body->forceX + body->forceY * body->forceY);
	float speedNorm = sqrt(pos->vx * pos->vx + pos->vy * pos->vy);

	((float*)args)[2] = (pos->vx > 0) ? 1 : -1;
	((float*)args)[4] = (pos->vy > 0) ? 1 : -1;

	if (speedNorm <= 0.07f && allForces <= staticCoef) {
		stopLeMassacre(pos, body, f, true, true);
		return false;
	};

	
	// printf("\ntimesOsciliatingX = %.2f, timesOsciliatingY = %.2f\n", timesOsciliatingX, timesOsciliatingY);
	// printf("lastSignX = %.2f, lastSignY = %.2f\n", lastSignX, lastSignY);
	// printf("New sign X = %.2f, New sign Y = %.2f\n", ((float*)args)[2], ((float*)args)[4]);

	if ((pos->vx > 0 && lastSignX < 0) || (pos->vx < 0 && lastSignX > 0)) {
		// printf(YELLOW "Increasing timesOsciliatingX: %.2f\n" RESET, timesOsciliatingX + 1);
		timesOsciliatingX++;
		((float*)args)[3] += 1.;

	} else if (fabs(pos->vx) > 0.3f) {
		timesOsciliatingX = 0;
		((float*)args)[3] = 0;
	}

	if ((pos->vy > 0 && lastSignY < 0) || (pos->vy < 0 && lastSignY > 0)) {
		// printf(YELLOW "Increasing timesOsciliatingY: %.2f\n" RESET, timesOsciliatingY + 1);
		timesOsciliatingY++;
		((float*)args)[5] += 1.;
	} else if (fabs(pos->vy) > 0.3f) {
		timesOsciliatingY = 0;
		((float*)args)[5] = 0;
	}

	Vector direction = {pos->vx, pos->vy};
	if (timesOsciliatingX >= seuil && timesOsciliatingY >= seuil) {
		stopLeMassacre(pos, body, f, true, true);
		return false;
	}
	if (timesOsciliatingX >= seuil) {
		stopLeMassacre(pos, body, f, true, false);
		f->Fy = - movingCoeff * direction.y;
		return false;
	}
	if (timesOsciliatingY >= seuil) {
		stopLeMassacre(pos, body, f, false, true);
		f->Fx = - movingCoeff * direction.x;
		return false;
	}

	normalize(&direction);
	f->Fx = - movingCoeff * direction.x;
	f->Fy = - movingCoeff * direction.y;

	// printf("norm = %.5f, pos->vx = %.5f, pos->vy = %.5f, coeff = %.5f\n", speedNorm, pos->vx, pos->vy, movingCoeff);
	// printf(BLUE "applying solid drag: force = %.5f N\n" RESET, sqrt(f->Fx * f->Fx + f->Fy * f->Fy));
	return false;
}


Force* create_force(ForceFunction func, void* additionalArgs, bool argsAreMalloc) {
	Force* f = malloc(sizeof(Force));
	f->func = func;
	f->Fx = 0;
	f->Fy = 0;
	f->additionalArgs = additionalArgs;
	f->argsAreMalloc = argsAreMalloc;
	return f;
}

bool update_entity_force(uint32_t entity, Force* f)
{
    return (*f->func)(entity, f, f->additionalArgs);
}

void free_force(Force* f) {
	if (f == NULL) return;
	if (f->argsAreMalloc)
		free(f->additionalArgs);
	free(f);
}
