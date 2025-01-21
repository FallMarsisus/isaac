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
void init_parent_component(ParentComponent* parent) {
    parent->children = create_array();
}
void init_child_component(ChildComponent* child, uint32_t parent) {
    child->parent = parent;
}

void add_child(ParentComponent* parent, uint32_t* id) {
    if(id) append(parent->children, id);
}

void free_parent_component(ParentComponent* parent) {
    if(parent) free_array(parent->children);
}
void free_all_other(uint32_t id, ECS_Manager* ecs) {
    ParentComponent* parent = ECS_GetComponent(ecs, id, PARENT);
    if(parent) free_parent_component(parent);
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