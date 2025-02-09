#include "enemies.h"

void init_enemy_ai(EnemyAiComponent* ai, uint32_t target) {
    ai->danger = calloc(8, sizeof(int));
    ai->interests = calloc(8, sizeof(int));
    
    ai->speed = 2;
    ai->target = target;
}
void free_enemy_ai(uint32_t id) {
    EnemyAiComponent* ai = ECS_GetComponent(id, ENEMYAI);

    if(!ai) return;
    free(ai->interests);
    free(ai->danger);
}

void update_enemy_ai(uint32_t id) {
    PositionComponent* pos = ECS_GetComponent(id, POSITION);
    SpriteComponent* sprite = ECS_GetComponent(id, SPRITE);
    EnemyAiComponent* ai = ECS_GetComponent(id, ENEMYAI);
    if(!ai || !pos || !sprite) return;

    PositionComponent* tPos = ECS_GetComponent(ai->target, POSITION);
    SpriteComponent* tSprite = ECS_GetComponent(ai->target, SPRITE);
    if(!tPos || !tSprite) return;

    Vector dir = {tPos->x - pos->x, tPos->y - pos->y};
    normalize(&dir);
    for(int i = 0; i < 7; i++) {
        float angle = i * (M_PI / 4);
        Vector dir2 = {cos(angle), sin(angle)};
        ai->interests[i] = scalaire(&dir, &dir2);
    }
}