#include "enemyStates.h"

void randomize_idle_vars(IdleStateVars* vars) {
    vars->direction = (Vector) {
        random_float(-1, 1),
        random_float(-1, 1)
    };
    normalize(&vars->direction);
    vars->speed = random_float(1, 5);
    vars->wander_time = random_float(0.5, 3);
}

IdleStateVars* create_idle_vars() {
    IdleStateVars* vars = malloc(sizeof(IdleStateVars));
    randomize_idle_vars(vars);
    return vars;
}
void on_idle_enter(State* state, uint32_t id) {
    IdleStateVars* vars = (IdleStateVars*) state->vars;
    randomize_idle_vars(vars);
}
void on_idle_update(State* state, uint32_t id) {
    PositionComponent* pos = ECS_GetComponent(id, POSITION);
    if(!pos) return;

    IdleStateVars* vars = (IdleStateVars*) state->vars;
    if(vars->wander_time > 0) {
        vars->wander_time -= 1/60.;
        
        pos->vx = vars->direction.x;
        pos->vy = vars->direction.y;
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