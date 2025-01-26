#pragma once

#include <stdlib.h>
#include <stdint.h>

#include "../../utils/betterArrays/string_dict.h"

typedef struct state_s State;

typedef void (*StateCallback)(State* state, uint32_t id);

struct state_s {
    char* name;
    StateCallback on_enter;
    StateCallback on_update;
    StateCallback on_exit;
};

typedef struct {
    Dictionary* state_dict;
    State* current_state;
    uint32_t id;
} StateMachineComponent;

State* create_state(char* name, StateCallback on_enter, StateCallback on_update, StateCallback on_exit);

void init_state_machine(StateMachineComponent* sm, uint32_t id);
void free_state_machine(StateMachineComponent* sm);

void add_state(StateMachineComponent* sm, State* state);

void switch_state(StateMachineComponent* sm, char* new_state);
void update_state_machine(StateMachineComponent* sm);