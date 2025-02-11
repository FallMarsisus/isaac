#include "enemyStates.h"

#define RAYCAST_STEP 32.0f
#define RAYCAST_RADIUS 48.0f

#define ATTACK_RANGE 300
#define ABANDON_RANGE 600

//Returns True if touch something
bool raycast(Vector start, Vector dir, float distance) {
    normalize(&dir);

    Vector pos = start;
    int steps = distance / RAYCAST_STEP;

    pos.x += dir.x * RAYCAST_STEP;
    pos.y += dir.y * RAYCAST_STEP;
    for (int i = 0; i < steps; i++) {
        pos.x += dir.x * RAYCAST_STEP;
        pos.y += dir.y * RAYCAST_STEP;

        for (int i = 0; i < ECS_GetManager()->st->dict->capacity; i++) {
            Node* current = ECS_GetManager()->st->dict->array[i];
            while (current) {
                PositionComponent* ePos = ECS_GetComponent(current->key, POSITION);
                RigidbodyComponent* eRb = ECS_GetComponent(current->key, BODY);
                if(!ePos || !eRb || eRb->is_dynamic || fabs(ePos->x - pos.x) > 2 * distance || fabs(ePos->y - pos.y) > 2 * distance) {
                    current = current->next;
                    continue;
                }
    
                if(checkCircleCollision(ePos, eRb, pos.x, pos.y, RAYCAST_RADIUS)) return true;
                current = current->next;
            }
        }
    }
    return false;
}

void randomize_idle_vars(IdleStateVars* vars) {
    vars->direction = (Vector) {
        random_float(-1, 1),
        random_float(-1, 1)
    };
    normalize(&vars->direction);
    vars->speed = random_float(0.5, 1.5);
    vars->wander_time = random_float(0.5, 3);
}
IdleStateVars* create_idle_vars(uint32_t pl) {
    IdleStateVars* vars = malloc(sizeof(IdleStateVars));
    vars->player = pl;
    randomize_idle_vars(vars);
    return vars;
}
void on_idle_enter(State* state, uint32_t id) {
    IdleStateVars* vars = (IdleStateVars*) state->vars;
    randomize_idle_vars(vars);
}
void on_idle_update(State* state, uint32_t id) {
    IdleStateVars* vars = (IdleStateVars*) state->vars;
    if(!vars) return;

    PositionComponent* posComp = ECS_GetComponent(id, POSITION);
    SpriteComponent* spriteComp = ECS_GetComponent(id, SPRITE);
    AnimationComponent* anim = ECS_GetComponent(id, ANIMATION);

    if(!posComp || !spriteComp) return;

    PositionComponent* playerPosCom = ECS_GetComponent(vars->player, POSITION);
    SpriteComponent* playerSpriteComp = ECS_GetComponent(vars->player, SPRITE);
    if(!playerPosCom || !playerSpriteComp) return;

    Vector pos = {posComp->x + spriteComp->width / 2, posComp->y + spriteComp->height / 2};
    Vector playerPos = {playerPosCom->x + playerSpriteComp->width / 2, playerPosCom->y + playerSpriteComp->height / 2};

    Vector dir = (Vector) {
        playerPos.x - pos.x,
        playerPos.y - pos.y
    };
    if(vectorSize(&dir) < ATTACK_RANGE && !raycast(pos, dir, vectorSize(&dir))) {
        StateChangeEvent* event = malloc(sizeof(StateChangeEvent));
        event->id = id; event->new_state = "chase";
        trigger_event(EVENT_STATE_CHANGE, event);
        return;
    }

    if(vars->wander_time > 0) {
        vars->wander_time -= 1/60.;

        normalize(&vars->direction);
        posComp->vx = vars->direction.x * vars->speed;
        posComp->vy = vars->direction.y * vars->speed;

        if(anim) {
            if(vars->direction.y < -0.5) set_active_anim(anim, 1);
            else if(vars->direction.y > 0.5) set_active_anim(anim, 0);
            else if(vars->direction.x < -0.5) set_active_anim(anim, 2);
            else if(vars->direction.x > 0.5) set_active_anim(anim, 3);
            play_anim(anim);
        }
    }
    else {
        randomize_idle_vars(vars);
    }
}
void on_idle_exit(State* state, uint32_t id) {}
void on_idle_free(State* state, uint32_t id) {
    IdleStateVars* vars = (IdleStateVars*) state->vars;
    if(vars) free(vars);
}

ChaseStateVars* create_chase_vars(uint32_t player) {
    ChaseStateVars* vars = malloc(sizeof(ChaseStateVars));
    vars->player = player;
    vars->speed = 3;
    return vars;
}
void on_chase_enter(State* state, uint32_t id) {}
void on_chase_update(State* state, uint32_t id) {
    ChaseStateVars* vars = (ChaseStateVars*) state->vars;
    if(!vars) return;

    PositionComponent* posComp = ECS_GetComponent(id, POSITION);
    SpriteComponent* spriteComp = ECS_GetComponent(id, SPRITE);
    RigidbodyComponent* rb = ECS_GetComponent(id, BODY);
    AnimationComponent* anim = ECS_GetComponent(id, ANIMATION);
    if(!posComp || !rb) return;

    PositionComponent* targetPosComp = ECS_GetComponent(vars->player, POSITION);
    SpriteComponent* targetSpriteComp = ECS_GetComponent(vars->player, SPRITE);
    if(!targetPosComp) return;

    Vector pos = {posComp->x + spriteComp->width / 2, posComp->y + spriteComp->height / 2};
    Vector targetPos = {targetPosComp->x + targetSpriteComp->width / 2, targetPosComp->y + targetSpriteComp->height / 2};
    
    Vector dir = (Vector) {
        targetPos.x - pos.x,
        targetPos.y - pos.y
    };
    if(vectorSize(&dir) < ATTACK_RANGE && raycast(pos, dir, vectorSize(&dir))) {
        StateChangeEvent* event = malloc(sizeof(StateChangeEvent));
        event->id = id; event->new_state = "follow";
        trigger_event(EVENT_STATE_CHANGE, event);
        return;
    }
    if(vectorSize(&dir) > ABANDON_RANGE) {
        StateChangeEvent* event = malloc(sizeof(StateChangeEvent));
        event->id = id; event->new_state = "idle";
        trigger_event(EVENT_STATE_CHANGE, event);
        return;
    }

    normalize(&dir);
    posComp->vx = dir.x * vars->speed;
    posComp->vy = dir.y * vars->speed;

    if(anim) {
        if(posComp->vy < 0) set_active_anim(anim, 1);
        else if(posComp->vy > 0) set_active_anim(anim, 0);
        else if(posComp->vx < 0) set_active_anim(anim, 2);
        else if(posComp->vx > 0) set_active_anim(anim, 3);
        play_anim(anim);
    }
}
void on_chase_exit(State* state, uint32_t id) {}
void on_chase_free(State* state, uint32_t id) {
    ChaseStateVars* vars = (ChaseStateVars*) state->vars;
    if(vars) free(vars);
}

FollowStateVars* create_follow_vars(uint32_t target, ID_array* entity_ids) {
    FollowStateVars* vars = malloc(sizeof(FollowStateVars));
    vars->target = target;
    vars->speed = 3;
    vars->prev_pos = create_queue();
    vars->prev_update = SDL_GetTicks();
    vars->entity_ids = entity_ids;
    return vars;
}
void on_follow_enter(State* state, uint32_t id) {
    FollowStateVars* vars = (FollowStateVars*) state->vars;
    if(!vars) return;

    PositionComponent* pos = ECS_GetComponent(id, POSITION);
    PositionComponent* targetPos = ECS_GetComponent(vars->target, POSITION);
    if(!pos || !targetPos) return;

    queue_clear(vars->prev_pos);

    vars->currentGoal = (Vector) {
        targetPos->x,
        targetPos->y
    };
}
void on_follow_update(State* state, uint32_t id) {
    FollowStateVars* vars = (FollowStateVars*) state->vars;
    if(!vars) return;

    PositionComponent* posComp = ECS_GetComponent(id, POSITION);
    SpriteComponent* spriteComp = ECS_GetComponent(id, SPRITE);
    RigidbodyComponent* rb = ECS_GetComponent(id, BODY);
    AnimationComponent* anim = ECS_GetComponent(id, ANIMATION);
    if(!posComp || !rb) return;

    PositionComponent* targetPosComp = ECS_GetComponent(vars->target, POSITION);
    SpriteComponent* targetSpriteComp = ECS_GetComponent(vars->target, SPRITE);
    if(!targetPosComp) return;

    Vector pos = {posComp->x + spriteComp->width / 2, posComp->y + spriteComp->height / 2};
    Vector targetPos = {targetPosComp->x + targetSpriteComp->width / 2, targetPosComp->y + targetSpriteComp->height / 2};

    Vector dirToTarget = (Vector) {
        targetPos.x - pos.x,
        targetPos.y - pos.y
    };
    if(vectorSize(&dirToTarget) < ATTACK_RANGE && !raycast(pos, dirToTarget, vectorSize(&dirToTarget))) {
        StateChangeEvent* event = malloc(sizeof(StateChangeEvent));
        event->id = id; event->new_state = "chase";
        trigger_event(EVENT_STATE_CHANGE, event);
        return;
    }
    if(vectorSize(&dirToTarget) > ABANDON_RANGE) {
        StateChangeEvent* event = malloc(sizeof(StateChangeEvent));
        event->id = id; event->new_state = "idle";
        trigger_event(EVENT_STATE_CHANGE, event);
        return;
    }

    if(vars->prev_update + 50 < SDL_GetTicks() || queue_is_empty(vars->prev_pos)) {
        vars->prev_update = SDL_GetTicks();
        queue_enqueue(vars->prev_pos, &targetPos, sizeof(Vector));
    }
    if(checkCircleCollision(posComp, rb, vars->currentGoal.x, vars->currentGoal.y, 16)) {
        queue_dequeue(vars->prev_pos, &vars->currentGoal, sizeof(Vector));
    }

    Vector dir = {vars->currentGoal.x - pos.x, vars->currentGoal.y - pos.y};
    if(vectorSize(&dir) > 0.1) {
        normalize(&dir);
        posComp->vx = dir.x * vars->speed;
        posComp->vy = dir.y * vars->speed;
    }

    if(anim) {
        if(posComp->vy < 0) set_active_anim(anim, 1);
        else if(posComp->vy > 0) set_active_anim(anim, 0);
        else if(posComp->vx < 0) set_active_anim(anim, 2);
        else if(posComp->vx > 0) set_active_anim(anim, 3);
        play_anim(anim);
    }
}
void on_follow_exit(State* state, uint32_t id) {
    FollowStateVars* vars = (FollowStateVars*) state->vars;
    if(!vars) return;

    PositionComponent* pos = ECS_GetComponent(id, POSITION);
    PositionComponent* targetPos = ECS_GetComponent(vars->target, POSITION);
    if(!pos || !targetPos) return;

    queue_clear(vars->prev_pos);

    vars->currentGoal = (Vector) {
        targetPos->x,
        targetPos->y
    };
}
void on_follow_free(State* state, uint32_t id) {
    FollowStateVars* vars = (FollowStateVars*) state->vars;
    if(vars) {
        queue_destroy(vars->prev_pos);
        free(vars);
    }
}