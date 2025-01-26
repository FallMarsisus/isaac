#include "teleport.h"

void update_teleporter(u_int32_t id) {
    PositionComponent* position = ECS_GetComponent(id, POSITION);
    SpriteComponent* sprite = ECS_GetComponent(id, SPRITE);
    TeleporterComponent* teleport = ECS_GetComponent(id, TELEPORT);

    if(position && sprite && teleport) {
        for(int i = 0; i < ECS_GetManager()->count; i++) {
            PositionComponent* otherPos = ECS_GetComponent(ECS_GetManager()->entity_ids[i], POSITION);
            SpriteComponent* otherSprite = ECS_GetComponent(ECS_GetManager()->entity_ids[i], SPRITE);
            PlayerMovementComponent* otherPlayer = ECS_GetComponent(ECS_GetManager()->entity_ids[i], PLAYER);

            if(otherPos && otherSprite && otherPlayer) {
                if (position->x < otherPos->x + otherSprite->width &&
                    position->x + sprite->width > otherPos->x &&
                    position->y < otherPos->y + otherSprite->height &&
                    position->y + sprite->height > otherPos->y) {
                    
                    otherPos->x = teleport->posX + otherPos->vx * 20;
                    otherPos->y = teleport->posY + otherPos->vy * 20;
                    return;
                }
            }
        }
    }
}