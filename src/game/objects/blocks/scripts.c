#include "scripts.h"

void update_teleporter(u_int32_t id, SDL_Rect cam) {
    PositionComponent* position = ECS_GetComponent(id, POSITION);
    SpriteComponent* sprite = ECS_GetComponent(id, SPRITE);
    TeleporterComponent* teleport = ECS_GetComponent(id, TELEPORT);

    if(position && sprite && teleport) {
        for (Entity e = ECS_GetFirstEntity(); e != -1; e = ECS_GetNextEntity(e)) {
            PositionComponent* otherPos = ECS_GetComponent(e, POSITION);
            SpriteComponent* otherSprite = ECS_GetComponent(e, SPRITE);
            PlayerMovementComponent* otherPlayer = ECS_GetComponent(e, PLAYER);

            if(otherPos && otherSprite && otherPlayer) {
                if (position->x < otherPos->x + otherSprite->width &&
                    position->x + sprite->width > otherPos->x &&
                    position->y < otherPos->y + otherSprite->height &&
                    position->y + sprite->height > otherPos->y) {
                    
                    int dx = (otherPos->vx > 0) - (otherPos->vx < 0);
                    int dy = (otherPos->vy > 0) - (otherPos->vy < 0);
                    otherPos->x = teleport->posX + dx * 100;
                    otherPos->y = teleport->posY + dy * 100;
                    return;
                }
            }
        }
    }
}

void update_trap(u_int32_t id, SDL_Rect cam) {
    PositionComponent* position = ECS_GetComponent(id, POSITION);
    SpriteComponent* sprite = ECS_GetComponent(id, SPRITE);
    AnimationComponent* animation = ECS_GetComponent(id, ANIMATION);
    RigidbodyComponent* body = ECS_GetComponent(id, BODY);

    if(position && sprite && animation) {
        if(animation->counter == 0 && !body->active) {
            body->active = true;
        }
        if(animation->counter == 1 && body->active) {
            body->active = false;
        }
    }
}
