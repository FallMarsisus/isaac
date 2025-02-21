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
    //printf("Changed states : %s\n", stateEvent->new_state);
    switch_state(sm, stateEvent->new_state);
}

void on_collision(Event event) {
    CollisionEvent* collision = (CollisionEvent*) event.data;
    if (collision->entity1 == collision->entity2) return;
    
    if (handle_collision_item(collision->entity1, collision->entity2)) {
        return;
    }
    
    RigidbodyComponent* body1 = ECS_GetComponent(collision->entity1, BODY);
    RigidbodyComponent* body2 = ECS_GetComponent(collision->entity2, BODY);
    if(!body1 || !body2) return;

    EffectComponent* effect1 = ECS_GetComponent(collision->entity1, EFFECT);
    if(effect1) {
        if(body1->is_dynamic && !body2->is_dynamic && effect1->has_physics) {
            // Effect is dynamic, wall is static
            // Remove effect
            ECS_RemoveEntity(collision->entity1);
        }
    }

    EffectComponent* effect2 = ECS_GetComponent(collision->entity2, EFFECT);
    if(effect2) {
        if(body2->is_dynamic && !body1->is_dynamic && effect2->has_physics) {
            // Effect is dynamic, wall is static
            // Remove effect
            ECS_RemoveEntity(collision->entity2);
        }
    }
    
    // gérer les autres cas après
}