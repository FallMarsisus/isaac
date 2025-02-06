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
            p1->x + r1->hitbox.x + r1->hitbox.w > p2->x + r2->hitbox.x&&
            p1->y + r1->hitbox.y < p2->y + r2->hitbox.y + r2->hitbox.h &&
            p1->y + r1->hitbox.y + r1->hitbox.h > p2->y + r2->hitbox.y);
}

void resolveAxis(PositionComponent* position, RigidbodyComponent* body, 
                 PositionComponent* otherPos, RigidbodyComponent* otherBody, 
                 float* velocity, char axis) {
    if (*velocity == 0) return;

    if (axis == 'x') {
        if (*velocity > 0) {
            position->x = otherPos->x - body->hitbox.w;
        } else if (*velocity < 0) {
            position->x = otherPos->x + otherBody->hitbox.w;
        }
    } else if (axis == 'y') {
        if (*velocity > 0) {
            position->y = otherPos->y - body->hitbox.h;
        } else if (*velocity < 0) {
            position->y = otherPos->y + otherBody->hitbox.h;
        }
    }

    *velocity = 0; // Stop movement along this axis
}

void resolveDynamicCollision(
    PositionComponent* p1, RigidbodyComponent* r1,
    PositionComponent* p2, RigidbodyComponent* r2
) {
    // Step 1: Calculate collision normal
    float dx = (p2->x + r2->hitbox.w / 2) - (p1->x + r1->hitbox.w / 2);
    float dy = (p2->y + r2->hitbox.h / 2) - (p1->y + r1->hitbox.h / 2);
    float distance = sqrt(dx * dx + dy * dy);

    if (distance == 0) return; // Prevent division by zero

    // Normalize the collision normal
    float nx = dx / distance;
    float ny = dy / distance;

    // Step 2: Resolve overlap (push entities apart)
    float overlap = 0.5f * (distance - r1->hitbox.w / 2 - r2->hitbox.w / 2);
    p1->x -= nx * overlap;
    p1->y -= ny * overlap;
    p2->x += nx * overlap;
    p2->y += ny * overlap;

    // Step 3: Relative velocity
    float vx = p2->vx - p1->vx;
    float vy = p2->vy - p1->vy;

    // Step 4: Calculate relative velocity along the normal
    float vn = vx * nx + vy * ny;

    // If velocities are separating, no impulse is needed
    if (vn > 0) return;

    // Step 5: Calculate impulse scalar based on the masses and restitution
    float mass1 = r1->mass;
    float mass2 = r2->mass;
    float restitution = fminf(r1->restitution, r2->restitution); // Use the lower restitution value
    float j = -(1 + restitution) * vn / (1 / mass1 + 1 / mass2);

    // Step 6: Apply impulse to velocities
    float impulseX = j * nx;
    float impulseY = j * ny;

    p1->vx -= impulseX / mass1;
    p1->vy -= impulseY / mass1;
    p2->vx += impulseX / mass2;
    p2->vy += impulseY / mass2;

    // Step 7: Frictional force (apply friction to reduce velocity along the tangent)
    float fx = vx - (vx * nx + vy * ny) * nx;
    float fy = vy - (vx * nx + vy * ny) * ny;
    float frictionMagnitude = sqrt(fx * fx + fy * fy) * fminf(r1->friction, r2->friction);

    if (frictionMagnitude > 0) {
        float frictionForceX = frictionMagnitude * (fx / sqrt(fx * fx + fy * fy));
        float frictionForceY = frictionMagnitude * (fy / sqrt(fx * fx + fy * fy));

        p1->vx -= frictionForceX / mass1;
        p1->vy -= frictionForceY / mass1;
        p2->vx += frictionForceX / mass2;
        p2->vy += frictionForceY / mass2;
    }
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
            for (int i = 0; i < ECS_GetManager()->st->dict->capacity; i++) {
                Node* current = ECS_GetManager()->st->dict->array[i];
                while (current) {
                    if(current->key == id) {
                        current = current->next;
                        continue;
                    }

                    PositionComponent* otherPos = ECS_GetComponent(current->key, POSITION);
                    RigidbodyComponent* otherBody = ECS_GetComponent(current->key, BODY);
                    if (!otherPos || !otherBody) {
                        current = current->next;
                        continue;
                    }

                    if (isColliding(position, body, otherPos, otherBody)) {
                        printf("Collided x!\n");
                        // Case Handling
                        if (!body->is_dynamic && !otherBody->is_dynamic) {
                            current = current->next;
                            continue;
                        } else if (body->is_dynamic && !otherBody->is_dynamic) {
                            // Dynamic vs. Static
                            position->x = originalX; // Revert X movement
                            resolveAxis(position, body, otherPos, otherBody, &position->vx, 'x');
                        } else if (!body->is_dynamic && otherBody->is_dynamic) {
                        current = current->next;
                        continue;
                        } else if (body->is_dynamic && otherBody->is_dynamic) {
                            // Dynamic vs. Dynamic: Apply elastic collision resolution
                            //resolveDynamicCollision(position, body, otherPos, otherBody);
                        }
                    }
                    current = current->next;
                }
            }
        }

        // Update Y-axis position next
        position->y += position->vy * 60 * delta;

        // Handle collisions for Y-axis
        if (body) {
            for (int i = 0; i < ECS_GetManager()->st->dict->capacity; i++) {
                Node* current = ECS_GetManager()->st->dict->array[i];
                while (current) {
                    if(current->key == id) {
                        current = current->next;
                        continue;
                    }

                    PositionComponent* otherPos = ECS_GetComponent(current->key, POSITION);
                    RigidbodyComponent* otherBody = ECS_GetComponent(current->key, BODY);
                    if (!otherPos || !otherBody) {
                        current = current->next;
                        continue;
                    }

                    if (isColliding(position, body, otherPos, otherBody)) {
                        // Case Handling
                        if (!body->is_dynamic && !otherBody->is_dynamic) {
                            current = current->next;
                            continue;
                        } else if (body->is_dynamic && !otherBody->is_dynamic) {
                            // Dynamic vs. Static
                            position->y = originalY; // Revert Y movement
                            resolveAxis(position, body, otherPos, otherBody, &position->vy, 'y');
                        } else if (!body->is_dynamic && otherBody->is_dynamic) {
                            current = current->next;
                            continue;
                        } else if (body->is_dynamic && otherBody->is_dynamic) {
                            // Dynamic vs. Dynamic: Apply elastic collision resolution
                            //resolveDynamicCollision(position, body, otherPos, otherBody);
                        }

                        
                    }
                    current = current->next;
                }
            }
        }
    }
}
