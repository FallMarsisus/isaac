#include "physicsSystems.h"

void init_rigidbody_component(RigidbodyComponent* body, int offsetX, int offsetY, int width, int height) {
    body->is_dynamic = false;
    body->hitbox = (SDL_Rect){offsetX, offsetY, width, height};
    body->friction = 0;
    body->restitution = 0;
    body->mass = 50;
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

void update_physics(uint32_t id, float delta) {
    PositionComponent* position = ECS_GetComponent(id, POSITION);
    RigidbodyComponent* body = ECS_GetComponent(id, BODY);

    if (position) {
        // Save original position
        float originalX = position->x;
        float originalY = position->y;

        // Update X-axis position first
        position->x += position->vx * 60 * delta;

        // Handle collisions for X-axis first
        if (body) {
            if(!body->is_dynamic) return;
            for (Entity e = ECS_GetFirstEntity(); e != -1; e = ECS_GetNextEntity(e)) {
                if(e == id) continue;

                PositionComponent* otherPos = ECS_GetComponent(e, POSITION);
                RigidbodyComponent* otherBody = ECS_GetComponent(e, BODY);
                if (!otherPos || !otherBody) continue;

                if (isColliding(position, body, otherPos, otherBody)) {
                    // Case Handling
                    if (!body->is_dynamic && !otherBody->is_dynamic) {
                        continue;
                    } else if (body->is_dynamic && !otherBody->is_dynamic) {
                        // Dynamic vs. Static
                        position->x = originalX; // Revert X movement
                        resolveAxis(position, body, otherPos, otherBody, &position->vx, 'x');
                    } else if (!body->is_dynamic && otherBody->is_dynamic) {
                        continue;
                    } else if (body->is_dynamic && otherBody->is_dynamic) {
                        // Dynamic vs. Dynamic: Apply elastic collision resolution
                        //resolveDynamicCollision(position, body, otherPos, otherBody);
                    }
                    
                    CollisionEvent* event = malloc(sizeof(CollisionEvent));
                    event->entity1 = id;
                    event->entity2 = e;
                    trigger_event(EVENT_COLLISION, event, true);
                }
            }
        }

        // Update Y-axis position next
        position->y += position->vy * 60 * delta;

        // Handle collisions for Y-axis
        if (body) {
            for (Entity e = ECS_GetFirstEntity(); e != -1; e = ECS_GetNextEntity(e)) {
                if(e == id) continue;

                PositionComponent* otherPos = ECS_GetComponent(e, POSITION);
                RigidbodyComponent* otherBody = ECS_GetComponent(e, BODY);
                if (!otherPos || !otherBody) continue;

                if (isColliding(position, body, otherPos, otherBody)) {
                    // Case Handling
                    if (!body->is_dynamic && !otherBody->is_dynamic) {
                        continue;
                    } else if (body->is_dynamic && !otherBody->is_dynamic) {
                        // Dynamic vs. Static
                        position->y = originalY; // Revert Y movement
                        resolveAxis(position, body, otherPos, otherBody, &position->vy, 'y');
                    } else if (!body->is_dynamic && otherBody->is_dynamic) {
                        continue;
                    } else if (body->is_dynamic && otherBody->is_dynamic) {
                        // Dynamic vs. Dynamic: Apply elastic collision resolution
                        //resolveDynamicCollision(position, body, otherPos, otherBody);
                    }
                    
                    CollisionEvent* event = malloc(sizeof(CollisionEvent));
                    event->entity1 = id;
                    event->entity2 = e;
                    trigger_event(EVENT_COLLISION, event, true);
                }
            }
        }
    }
}
