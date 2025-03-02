#include "scripts.h"

typedef struct {
    int posX;
    int posY;
} TeleporterData;

typedef struct {
    int last_change;
    int change_time;

    bool active;
} TrapData;

void init_teleporter(ScriptComponent* script, int posX, int posY) {
    TeleporterData* teleport = malloc(sizeof(TeleporterData));
    teleport->posX = posX;
    teleport->posY = posY;

    script->data = teleport;
    script->update = update_teleporter;
}
void init_trap(ScriptComponent* script) {
    TrapData* trap = malloc(sizeof(TrapData));
    trap->active = false;
    trap->last_change = SDL_GetTicks();
    trap->change_time = 1000;

    script->data = trap;
    script->update = update_trap;
}

void update_teleporter(u_int32_t id, SDL_Rect cam, uint32_t* entities, int amount) {
    PositionComponent* position = ECS_GetComponent(id, POSITION);
    SpriteComponent* sprite = ECS_GetComponent(id, SPRITE);

    ScriptComponent* script = ECS_GetComponent(id, SCRIPT);
    TeleporterData* teleport = (TeleporterData*)script->data;

    if(position && sprite && teleport) {
        for (int i = 0; i < amount; i++) {
            if(entities[i] == id) continue;

            RigidbodyComponent* body = ECS_GetComponent(entities[i], BODY);
            if(!body || !body->layer) continue;

            PositionComponent* otherPos = ECS_GetComponent(entities[i], POSITION);
            SpriteComponent* otherSprite = ECS_GetComponent(entities[i], SPRITE);

            if(otherPos && otherSprite) {
                if (position->x < otherPos->x + otherSprite->width &&
                    position->x + sprite->width > otherPos->x &&
                    position->y < otherPos->y + otherSprite->height &&
                    position->y + sprite->height > otherPos->y) {
                    printf("Teleporter at %d %d\n", teleport->posX, teleport->posY);
                    
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
    RigidbodyComponent* body = ECS_GetComponent(id, BODY);
    ScriptComponent* script = ECS_GetComponent(id, SCRIPT);
    if(!script) return;

    TileComponent* tile = ECS_GetComponent(id, TILE);
    if(!tile) return;

    TrapData* trap = (TrapData*)script->data;

    if(position && sprite && tile) {
        if(SDL_GetTicks() - trap->last_change > trap->change_time) {
            trap->active = !trap->active;
            trap->last_change = SDL_GetTicks();

            tile->tile_x = trap->active ? 4 : 5;
        }
    }
}
