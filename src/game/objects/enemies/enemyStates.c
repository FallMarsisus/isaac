#include "enemyStates.h"

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