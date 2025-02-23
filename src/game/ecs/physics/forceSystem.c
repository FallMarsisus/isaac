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

bool knockback_force(uint32_t entity, Force* f, void* args) {
    PositionComponent* pos = ECS_GetComponent(entity, POSITION);
    RigidbodyComponent* body = ECS_GetComponent(entity, BODY);
    
    if (!pos || !body) return true;
    
    static float decay = 0.90f;
    static float velocityFactor = 0.02f;
    
    // Stocker la force initiale dans additionalArgs[3] si pas encore fait
    float* forceArgs = (float*)f->additionalArgs;
    float dirX = forceArgs[0];
    float dirY = forceArgs[1];
    if (forceArgs[2] <= 0) return true;  // Force épuisée
    
    // Appliquer la décroissance
    forceArgs[2] *= decay;
    
    // Appliquer la force
    f->Fx = forceArgs[2] * dirX;
    f->Fy = forceArgs[2] * dirY;
    
    // Mise à jour de la vélocité
    pos->vx = f->Fx * velocityFactor;
    pos->vy = f->Fy * velocityFactor;
    
    printf("Force remaining: %f\n", forceArgs[2]);
    
    return (forceArgs[2] < 10.0f);  // Arrêter quand la force devient très faible
}

bool solid_drag_force(uint32_t entity, Force* f, void* args) {
	PositionComponent* pos = ECS_GetComponent(entity, POSITION);
	
	if (!pos) return true;

	float coeff = ((float*)args)[0];

	if (fabs(pos->vx) <= 0.001f && fabs(pos->vy) <= 0.001f) return true;

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
    if (f->additionalArgs) {
        free(f->additionalArgs);
    }
    free(f);
}
