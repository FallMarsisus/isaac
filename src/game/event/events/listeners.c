#include "listeners.h"

void on_player_move(Event event) {
    PlayerMovedEvent* data = (PlayerMovedEvent*) event.data;
    printf("Entity %d moved : %f / %f\n", data->player_id, data->new_x, data->new_y);
}

void on_chest_open(Event event) {
    ChestOpenedEvent* chestOpenedEvent = (ChestOpenedEvent*)event.data;
    printf("Chest %d opened at (%f, %f)!\n", chestOpenedEvent->chest_id, chestOpenedEvent->x, chestOpenedEvent->y);
    InventoryComponent* inv = ECS_GetComponent(chestOpenedEvent->player_id, INVENT);
    add_item_to_inventory(chestOpenedEvent->player_id, apple );
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
    if (!collision || collision->entity1 == collision->entity2) return;
    
    if (handle_collision_item(collision->entity1, collision->entity2)) {
        return;
    }

    // Vérifier les composants
    PositionComponent* pos1 = ECS_GetComponent(collision->entity1, POSITION);
    RigidbodyComponent* body1 = ECS_GetComponent(collision->entity1, BODY);
    HealthComponent* health1 = ECS_GetComponent(collision->entity1, HEALTH);

    PositionComponent* pos2 = ECS_GetComponent(collision->entity2, POSITION);
    RigidbodyComponent* body2 = ECS_GetComponent(collision->entity2, BODY);
    DamagerComponent* damager2 = ECS_GetComponent(collision->entity2, DAMAGER);

    if (pos1 && pos2 && body1 && body2) {
        TileComponent* tile = ECS_GetComponent(collision->entity2, TILE);

        if(tile && tile->tile_x == 8 && tile->tile_y == 1) {
            tile->tile_x = 9;
            
            ChestOpenedEvent* event = malloc(sizeof(ChestOpenedEvent));
            event->chest_id = collision->entity2;
            event->player_id = collision->entity1;
            event->x = pos2->x; event->y = pos2->y;
            trigger_event(EVENT_CHEST_OPENED, event, true);
        }
        if(damager2 && health1) {
            if (body1->layer == 2 && damager2->damage_player) {
                if(tile) { 
                    if(tile->tile_x == 4 && tile->tile_y == 1) {
                        ScriptComponent* script = ECS_GetComponent(collision->entity1, SCRIPT);
                        if(script && script->data) {
                            TrapData* tdata = script->data;
                            if(tdata && tdata->active) {
                                damage(collision->entity1, collision->entity2);
                            }
                        }
                    }
                }
                else {
                    damage(collision->entity1, collision->entity2);
                }
            }
            else if (body1->layer == 1 && !damager2->damage_player) {
                damage(collision->entity1, collision->entity2);
            }
        }
    }

    // Gérer les effets
    EffectComponent* effect1 = ECS_GetComponent(collision->entity1, EFFECT);
    if(effect1 && body1 && body2) {
        if(body1->is_dynamic && !body2->is_dynamic && effect1->has_physics) {
            ECS_RemoveEntity(collision->entity1);
        }
    }

    EffectComponent* effect2 = ECS_GetComponent(collision->entity2, EFFECT);
    if(effect2 && body1 && body2) {
        if(body2->is_dynamic && !body1->is_dynamic && effect2->has_physics) {
            ECS_RemoveEntity(collision->entity2);
        }
    }
}