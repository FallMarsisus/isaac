#include "scripts.h"

typedef struct {
    int posX;
    int posY;
} TeleporterData;

void init_teleporter(ScriptComponent* script, int posX, int posY) {
    TeleporterData* teleport = malloc(sizeof(TeleporterData));
    teleport->posX = posX;
    teleport->posY = posY;

    script->data = teleport;
    script->update = update_teleporter;
}

void init_trap(ScriptComponent* script) {
    script->data = NULL;
    script->update = update_trap;
}

void update_teleporter(u_int32_t id, SDL_Rect cam, uint32_t* entities, int amount) {
    PositionComponent* position = ECS_GetComponent(id, POSITION);
    SpriteComponent* sprite = ECS_GetComponent(id, SPRITE);

    ScriptComponent* script = ECS_GetComponent(id, SCRIPT);
    TeleporterData* teleport = (TeleporterData*)script->data;

    if(position && sprite && teleport) {
        for (int i = 0; i < amount; i++) {
            PositionComponent* otherPos = ECS_GetComponent(entities[i], POSITION);
            SpriteComponent* otherSprite = ECS_GetComponent(entities[i], SPRITE);

            if(otherPos && otherSprite) {
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

void update_trap(u_int32_t id, SDL_Rect cam, uint32_t* entities, int amount) {
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
