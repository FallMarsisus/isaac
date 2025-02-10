#include "enemyStates.h"

#define DIRECTION_COUNT 8
#define RAYCAST_STEP 8.0f
#define RAYCAST_RADIUS 16.0f
#define MAX_RAYCAST_DISTANCE 48.0f

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

    PositionComponent* pos = ECS_GetComponent(id, POSITION);
    AnimationComponent* anim = ECS_GetComponent(id, ANIMATION);
    PositionComponent* playerPos = ECS_GetComponent(vars->player, POSITION);

    if(!pos || !playerPos) return;
    
    Vector dir = (Vector) {
        playerPos->x - pos->x,
        playerPos->y - pos->y
    };
    if(vectorSize(&dir) < 250) {
        StateChangeEvent* event = malloc(sizeof(StateChangeEvent));
        event->id = id; event->new_state = "chase";
        trigger_event(EVENT_STATE_CHANGE, event);
        return;
    }

    if(vars->wander_time > 0) {
        vars->wander_time -= 1/60.;

        normalize(&vars->direction);
        pos->vx = vars->direction.x * vars->speed;
        pos->vy = vars->direction.y * vars->speed;

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

    PositionComponent* pos = ECS_GetComponent(id, POSITION);
    AnimationComponent* anim = ECS_GetComponent(id, ANIMATION);
    PositionComponent* playerPos = ECS_GetComponent(vars->player, POSITION);

    if(!pos || !playerPos) return;
    
    Vector dir = (Vector) {
        playerPos->x - pos->x,
        playerPos->y - pos->y
    };
    if(vectorSize(&dir) > 250) {
        StateChangeEvent* event = malloc(sizeof(StateChangeEvent));
        event->id = id; event->new_state = "idle";
        trigger_event(EVENT_STATE_CHANGE, event);
    }

    normalize(&dir);
    pos->vx = dir.x * vars->speed;
    pos->vy = dir.y * vars->speed;
    if(anim) {
        if(pos->vy < 0) set_active_anim(anim, 1);
        else if(pos->vy > 0) set_active_anim(anim, 0);
        else if(pos->vx < 0) set_active_anim(anim, 2);
        else if(pos->vx > 0) set_active_anim(anim, 3);
        play_anim(anim);
    }
}
void on_chase_exit(State* state, uint32_t id) {}
void on_chase_free(State* state, uint32_t id) {
    ChaseStateVars* vars = (ChaseStateVars*) state->vars;
    if(vars) free(vars);
}

FollowStateVars* create_follow_vars(uint32_t target) {
    FollowStateVars* vars = malloc(sizeof(FollowStateVars));
    vars->target = target;
    vars->speed = 2;
    vars->prev_pos = create_queue();
    vars->prev_update = SDL_GetTicks();
    return vars;
}
void on_follow_enter(State* state, uint32_t id) {
    FollowStateVars* vars = (FollowStateVars*) state->vars;
    if(!vars) return;

    PositionComponent* pos = ECS_GetComponent(id, POSITION);
    PositionComponent* targetPos = ECS_GetComponent(vars->target, POSITION);
    if(!pos || !targetPos) return;

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

    if(vars->prev_update + 100 < SDL_GetTicks() || queue_is_empty(vars->prev_pos)) {
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
void on_follow_exit(State* state, uint32_t id) {}
void on_follow_free(State* state, uint32_t id) {
    FollowStateVars* vars = (FollowStateVars*) state->vars;
    if(vars) {
        queue_destroy(vars->prev_pos);
        free(vars);
    }
}