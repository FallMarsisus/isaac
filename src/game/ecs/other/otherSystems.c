#include "otherSystems.h"

void init_timer_component(TimerComponent* timer, float chrono) {
    timer->last = SDL_GetTicks();
    timer->time = chrono;
}
void init_target_component(TargetMovementComponent* targetComp, u_int32_t target) {
    targetComp->entity = target;
    targetComp->speed = 2;
}
void init_script_component(ScriptComponent* script, void (*update)(uint32_t entity, ECS_Manager* ecs)) {
    script->update = update;
}

void update_others(uint32_t id, ECS_Manager* ecs) {
    PositionComponent* position = ECS_GetComponent(ecs, id, POSITION);
    TargetMovementComponent* targetComp = ECS_GetComponent(ecs, id, TARGET);
    ScriptComponent* script = ECS_GetComponent(ecs, id, SCRIPT);

    if(script) {
        script->update(id, ecs);
    }

    if (position && targetComp) {
        PositionComponent* target_pos = ECS_GetComponent(ecs, targetComp->entity, POSITION);
        if(target_pos) {
            float dx = target_pos->x - position->x;
            float dy = target_pos->y - position->y;
            float distance = sqrt(pow(dx, 2) + pow(dy, 2));

            if(distance > 0.1) {
                position->vx = (dx / distance) * targetComp->speed;
                position->vy = (dy / distance) * targetComp->speed;
            }
            else {
                position->vx = 0; position->vy = 0;
            }
        }
    }
}