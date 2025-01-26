#include "stateMachine.h"

State* create_state(char* name, StateCallback on_enter, StateCallback on_update, StateCallback on_exit) {
    State* state = (State*)malloc(sizeof(State));
    if (!state) {
        printf("Failed to allocate memory for state: %s\n", name);
        exit(1);
    }

    state->name = name;
    state->on_enter = on_enter;
    state->on_update = on_update;
    state->on_exit = on_exit;
    return state;
}

void init_state_machine(StateMachineComponent* sm, uint32_t id) {
    sm->id = id;
    sm->current_state = NULL;
    sm->state_dict = create_dictionary();
}
void free_state_machine(StateMachineComponent* sm) {
    free_dictionary(sm->state_dict);
    
    free(sm);
}

void add_state(StateMachineComponent* sm, State* state) {
    if(!sm || !state) return;
    add_to_dictionary(sm->state_dict, state->name, state);
}

void switch_state(StateMachineComponent* sm, char* name) {
    if (!sm) return;
    if (sm->current_state && sm->current_state->on_exit) {
        sm->current_state->on_exit(sm->current_state, sm->id);
    }

    State* new_state = get_from_dictionary(sm->state_dict, name);

    sm->current_state = new_state;

    // Enter the new state
    if (new_state && new_state->on_enter) {
        new_state->on_enter(new_state, sm->id);
    }
}
void update_state_machine(StateMachineComponent* sm) {
    if (!sm || !sm->current_state || !sm->current_state->on_update) return;
    sm->current_state->on_update(sm->current_state, sm->id);
}