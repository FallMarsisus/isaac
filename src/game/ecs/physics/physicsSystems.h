#pragma once

#include "../components.h"
#include "../ecs.h"

void init_rigidbody_component(RigidbodyComponent* body, int offsetX, int offsetY, int width, int height);

bool isColliding(PositionComponent* p1, RigidbodyComponent* r1, PositionComponent* p2, RigidbodyComponent* r2);

void update_physics(uint32_t id, ECS_Manager* ecs);