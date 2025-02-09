#include "enemyAi.h"
#include <math.h>
#include <stdlib.h>

#define DIRECTION_COUNT 8
#define RAYCAST_STEP 8.0f
#define RAYCAST_RADIUS 16.0f
#define MAX_RAYCAST_DISTANCE 48.0f

void init_enemy_ai_component(EnemyAiComponent* ai, uint32_t target) {
    ai->speed = 2;
    ai->target = target;
    PositionComponent* targetPosComp = ECS_GetComponent(ai->target, POSITION);
    if(!targetPosComp) return;

    ai->currentGoal = (Vector) {targetPosComp->x, targetPosComp->y};

    ai->prev_pos = create_queue();
    ai->prev_update = SDL_GetTicks();
}

void free_enemy_ai(uint32_t id) {
    EnemyAiComponent* ai = ECS_GetComponent(id, ENEMYAI);
    if (!ai) return;
    queue_destroy(ai->prev_pos);
}

// Raycast function to check if path is clear considering sprite size
int raycast(Vector start, Vector dir, float distance, uint32_t* entities, int amount) {
    normalize(&dir);

    Vector current = start;
    int steps = distance / RAYCAST_STEP;
    for (int i = 0; i < steps; i++) {
        current.x += dir.x * RAYCAST_STEP;
        current.y += dir.y * RAYCAST_STEP;
        
        for (int j = 0; j < amount; j++) {
            PositionComponent* ePos = ECS_GetComponent(entities[j], POSITION);
            RigidbodyComponent* eRb = ECS_GetComponent(entities[j], BODY);
            if (!ePos || !eRb || eRb->is_dynamic) continue;

            if(checkCircleCollision(ePos, eRb, current.x, current.y, RAYCAST_RADIUS)) return 0;
        }
    }
    return 1;
}

void update_enemy_ai(uint32_t id, uint32_t* entities, int amount) {
    PositionComponent* posComp = ECS_GetComponent(id, POSITION);
    SpriteComponent* spriteComp = ECS_GetComponent(id, SPRITE);
    RigidbodyComponent* rigidComp = ECS_GetComponent(id, BODY);
    EnemyAiComponent* ai = ECS_GetComponent(id, ENEMYAI);
    if(!ai || !posComp || !spriteComp || !rigidComp) return;

    PositionComponent* targetPosComp = ECS_GetComponent(ai->target, POSITION);
    SpriteComponent* targetSpriteComp = ECS_GetComponent(ai->target, SPRITE);
    if(!targetPosComp || !targetSpriteComp) return;

    Vector pos = {posComp->x + spriteComp->width / 2, posComp->y + spriteComp->height / 2};
    Vector targetPos = {targetPosComp->x + targetSpriteComp->width / 2, targetPosComp->y + targetSpriteComp->height / 2};
    if(ai->prev_update + 100 < SDL_GetTicks() || queue_is_empty(ai->prev_pos)) {
        ai->prev_update = SDL_GetTicks();

        queue_enqueue(ai->prev_pos, &targetPos, sizeof(Vector));
    }

    if(checkCircleCollision(posComp, rigidComp, ai->currentGoal.x, ai->currentGoal.y, 16)) {
        queue_dequeue(ai->prev_pos, &ai->currentGoal, sizeof(Vector));
    }

    Vector dir = {ai->currentGoal.x - pos.x, ai->currentGoal.y - pos.y};
    if(vectorSize(&dir) > 0.1) {
        normalize(&dir);
        posComp->vx = dir.x * ai->speed;
        posComp->vy = dir.y * ai->speed;
    }
}
