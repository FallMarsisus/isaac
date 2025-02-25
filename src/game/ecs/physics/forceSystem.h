#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef struct Force_s Force;

typedef bool (*ForceFunction)(uint32_t entity, Force* f, void* args);

struct Force_s {
	float Fx;
	float Fy;
	ForceFunction func;
	void* additionalArgs;
	bool argsAreMalloc;
};

typedef struct sdfArgs_s {
	float staticCoef;
	float movingCoef;
	float lastSignX;
	int timesOsciliatingX;
	float lastSignY;
	int timesOsciliatingY;
} sdfArgs;

// current forces created
bool wind_force(uint32_t entity, Force* f, void* args);
bool fluid_drag_force(uint32_t entity, Force* f, void* args);
bool knockback_force(uint32_t entity, Force* f, void* args);
bool solid_drag_force(uint32_t entity, Force* f, void* args);


// functions of the data type
Force* create_force(ForceFunction func, void* additionalArgs);
bool update_entity_force(uint32_t entity, Force* f);
void free_force(Force* f);