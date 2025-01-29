#include "listeners.h"

void on_player_move(Event event) {
    PlayerMovedEvent* data = (PlayerMovedEvent*) event.data;
    printf("Entity %d moved : %f / %f\n", data->player_id, data->new_x, data->new_y);
}

void on_chest_open(Event event) {
    ChestOpenedEvent* chestOpenedEvent = (ChestOpenedEvent*)event.data;
    printf("Chest %d opened at (%f, %f)!\n", chestOpenedEvent->chest_id, chestOpenedEvent->x, chestOpenedEvent->y);
    ECS_RemoveEntity(chestOpenedEvent->chest_id);
}

void on_state_change(Event event) {
    StateChangeEvent* stateEvent = (StateChangeEvent*) event.data;
    StateMachineComponent* sm = ECS_GetComponent(stateEvent->id, STATE_MACHINE);

    switch_state(sm, stateEvent->new_state);
}