#include "physicsSystems.h"

void init_position_component(PositionComponent* position, float x, float y) {
    position->x = x;
    position->y = y;
    position->vx = 0;
    position->vy = 0;
    position->ax = 0;
    position->ay = 0;
    position->camFixed = false;
}
void init_rigidbody_component(RigidbodyComponent* body, int offsetX, int offsetY, int width, int height) {
    body->is_dynamic = false;
    body->hitbox = (SDL_Rect){offsetX, offsetY, width, height};
    body->friction = 0;
    body->restitution = 0;
    body->mass = 50;
    body->forceX = 0;
    body->forceY = 0;
	body->forces = create_array();
}

bool isColliding(PositionComponent* p1, RigidbodyComponent* r1, 
                 PositionComponent* p2, RigidbodyComponent* r2) {
    return (p1->x + r1->hitbox.x < p2->x + r2->hitbox.x + r2->hitbox.w &&
            p1->x + r1->hitbox.x + r1->hitbox.w > p2->x + r2->hitbox.x &&
            p1->y + r1->hitbox.y < p2->y + r2->hitbox.y + r2->hitbox.h &&
            p1->y + r1->hitbox.y + r1->hitbox.h > p2->y + r2->hitbox.y);
}

bool checkCircleCollision(PositionComponent* p1, RigidbodyComponent* r1, 
                          float cX, float cY, float radius) {
    float closestX = fmaxf(p1->x + r1->hitbox.x, fminf(cX, p1->x + r1->hitbox.x + r1->hitbox.w));
    float closestY = fmaxf(p1->y + r1->hitbox.y, fminf(cY, p1->y + r1->hitbox.y + r1->hitbox.h));
    float distanceX = cX - closestX;
    float distanceY = cY - closestY;
    return (distanceX * distanceX + distanceY * distanceY) <= (radius * radius);
}

void resolveAxis(PositionComponent* position, RigidbodyComponent* body, 
    PositionComponent* otherPos, RigidbodyComponent* otherBody, 
    float* velocity, char axis) {
    if (*velocity == 0) return;

    if (axis == 'x') {
        if (*velocity > 0) {
            position->x = otherPos->x + otherBody->hitbox.x - body->hitbox.w - body->hitbox.x;
        } else if (*velocity < 0) {
            position->x = otherPos->x + otherBody->hitbox.x + otherBody->hitbox.w - body->hitbox.x;
        }
    } else if (axis == 'y') {
        if (*velocity > 0) {
            position->y = otherPos->y + otherBody->hitbox.y - body->hitbox.h - body->hitbox.y;
        } else if (*velocity < 0) {
            position->y = otherPos->y + otherBody->hitbox.y + otherBody->hitbox.h - body->hitbox.y;
        }
    }

    *velocity = 0;
}

void apply_one_force(RigidbodyComponent* body, float fx, float fy) {
	// printf("Applying force: Fx = %f, Fy = %f\n", fx, fy);
	body->forceX += fx;
	body->forceY += fy;
}

void apply_all_forces(uint32_t entity, RigidbodyComponent* body) {
	bool shouldBeRemoved;
	Force* currentForce;

	if (!body->forces || get_len(body->forces) == 0) return;
	
	for (int i = 0; i < get_len(body->forces); i++) {

		currentForce = (Force*) get_elt(body->forces, i);
		shouldBeRemoved = update_entity_force(entity, currentForce);
		apply_one_force(body, currentForce->Fx, currentForce->Fy);

		if (shouldBeRemoved) {
			// printf("Removing force: Fx = %f, Fy = %f\n", currentForce->Fx, currentForce->Fy);
			free_force(get_elt(body->forces, i));
			remove_dynarr(body->forces, i);
			i--;
		}
	}

	// printf("Sum of forces: Fx = %f, Fy = %f\n\n", body->forceX, body->forceY);
}

void add_force(uint32_t entity, Force* f) {
	RigidbodyComponent* body = ECS_GetComponent(entity, BODY);
	if (!body) return;

	append(body->forces, f);
}

void update_physics(uint32_t id, float delta) {
    PositionComponent* position = ECS_GetComponent(id, POSITION);
    RigidbodyComponent* body = ECS_GetComponent(id, BODY);
    
    if (body) apply_all_forces(id, body);

    if (position && body && body->is_dynamic) {

        if (body->mass > 0) {
            position->ax = body->forceX / body->mass;
            position->ay = body->forceY / body->mass;
            body->forceX = 0;
            body->forceY = 0;
        }

        float originalX = position->x;
        float originalY = position->y;

        if (fabsf(position->ax) > 0.001f) {
            position->vx += position->ax * delta;
            if (fabsf(position->vx) < 0.001f) position->vx = 0;
        }
        if (fabsf(position->ay) > 0.001f) {
            position->vy += position->ay * delta;
            if (fabsf(position->vy) < 0.001f) position->vy = 0;
        }
        if (sqrtf(position->vx * position->vx + position->vy * position->vy) < 0.006f) {
            position->vx = 0;
            position->vy = 0;
        }

        // Array to track collisions already triggered with other entities this update.
        // Assuming a maximum of 128 collisions per update.
        int collidedEntities[128];
        int collidedCount = 0;

        // Process X-axis collisions
        position->x += position->vx * 60 * delta;
        for (Entity e = ECS_GetFirstEntity(); e != -1; e = ECS_GetNextEntity(e)) {
            if (e == id) continue;
            
            PositionComponent* otherPos = ECS_GetComponent(e, POSITION);
            RigidbodyComponent* otherBody = ECS_GetComponent(e, BODY);
            if (!otherPos || !otherBody) continue;

            if (isColliding(position, body, otherPos, otherBody)) {
                if (body->is_dynamic && !otherBody->is_dynamic) {
                    position->x = originalX;
                    resolveAxis(position, body, otherPos, otherBody, &position->vx, 'x');
                }
                // Only trigger collision event from one side and only once per collision pair.
                if (id < e) {
                    bool alreadyTriggered = false;
                    for (int i = 0; i < collidedCount; i++) {
                        if (collidedEntities[i] == e) {
                            alreadyTriggered = true;
                            break;
                        }
                    }
                    if (!alreadyTriggered) {
                        CollisionEvent* event = malloc(sizeof(CollisionEvent));
                        event->entity1 = id;
                        event->entity2 = e;
                        trigger_event(EVENT_COLLISION, event, true);
                        collidedEntities[collidedCount++] = e;
                    }
                }
            }
        }

        // Process Y-axis collisions
        position->y += position->vy * 60 * delta;
        for (Entity e = ECS_GetFirstEntity(); e != -1; e = ECS_GetNextEntity(e)) {
            if (e == id) continue;
            
            PositionComponent* otherPos = ECS_GetComponent(e, POSITION);
            RigidbodyComponent* otherBody = ECS_GetComponent(e, BODY);
            if (!otherPos || !otherBody) continue;

            if (isColliding(position, body, otherPos, otherBody)) {
                if (body->is_dynamic && !otherBody->is_dynamic) {
                    position->y = originalY;
                    resolveAxis(position, body, otherPos, otherBody, &position->vy, 'y');
                }
                if (id < e) {
                    bool alreadyTriggered = false;
                    for (int i = 0; i < collidedCount; i++) {
                        if (collidedEntities[i] == e) {
                            alreadyTriggered = true;
                            break;
                        }
                    }
                    if (!alreadyTriggered) {
                        CollisionEvent* event = malloc(sizeof(CollisionEvent));
                        event->entity1 = id;
                        event->entity2 = e;
                        trigger_event(EVENT_COLLISION, event, true);
                        collidedEntities[collidedCount++] = e;
                    }
                }
            }
        }
    }
}
