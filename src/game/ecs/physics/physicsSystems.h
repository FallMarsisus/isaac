#pragma once

#include "../components.h"
#include "../ecs.h"
#include "./forceSystem.h"

#include "../../event/eventList.h"
#include "../../event/eventSystem.h"

void init_position_component(PositionComponent* position, float x, float y);
void init_rigidbody_component(RigidbodyComponent* body, int offsetX, int offsetY, int width, int height);

void free_rigidbody_component(uint32_t entity);

void apply_one_force(RigidbodyComponent* body, float fx, float fy);

void add_force(uint32_t entity, Force* f);
void apply_all_forces(uint32_t entity, RigidbodyComponent* body);
bool isColliding(PositionComponent* p1, RigidbodyComponent* r1, PositionComponent* p2, RigidbodyComponent* r2);
bool checkCircleCollision(PositionComponent* p1, RigidbodyComponent* r1, float cX, float cY, float radius);

void update_physics(uint32_t id, uint32_t* entities, int amount, float delta);