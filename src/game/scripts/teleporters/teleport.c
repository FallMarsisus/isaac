#include "teleport.h"

void update_teleporter(u_int32_t id, SDL_Rect cam) {
    PositionComponent* position = ECS_GetComponent(id, POSITION);
    SpriteComponent* sprite = ECS_GetComponent(id, SPRITE);
    TeleporterComponent* teleport = ECS_GetComponent(id, TELEPORT);

    if(position && sprite && teleport) {
        for (int i = 0; i < ECS_GetManager()->st->dict->capacity; i++) {
            Node* current = ECS_GetManager()->st->dict->array[i];
            while (current) {    
                PositionComponent* otherPos = ECS_GetComponent(current->key, POSITION);
                SpriteComponent* otherSprite = ECS_GetComponent(current->key, SPRITE);
                PlayerMovementComponent* otherPlayer = ECS_GetComponent(current->key, PLAYER);

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
                current = current->next;
            }
        }
    }
}