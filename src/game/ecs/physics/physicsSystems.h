#pragma once

#include "../components.h"
#include "../ecs.h"

#include "../../event/eventList.h"
#include "../../event/eventSystem.h"

void init_position_component(PositionComponent* position, float x, float y);
void init_rigidbody_component(RigidbodyComponent* body, int offsetX, int offsetY, int width, int height);

void apply_force(RigidbodyComponent* body, float fx, float fy);

bool isColliding(PositionComponent* p1, RigidbodyComponent* r1, PositionComponent* p2, RigidbodyComponent* r2);
bool checkCircleCollision(PositionComponent* p1, RigidbodyComponent* r1, float cX, float cY, float radius);

void update_physics(uint32_t id, float delta);