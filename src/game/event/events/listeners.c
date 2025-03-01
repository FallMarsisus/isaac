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
    if (!collision || collision->entity1 == collision->entity2) return;
    
    if (handle_collision_item(collision->entity1, collision->entity2)) {
        return;
    }

    // Vérifier les composants
    DamagerComponent* damager1 = ECS_GetComponent(collision->entity1, DAMAGER);
    DamagerComponent* damager2 = ECS_GetComponent(collision->entity2, DAMAGER);
    HealthComponent* health1 = ECS_GetComponent(collision->entity1, HEALTH);
    HealthComponent* health2 = ECS_GetComponent(collision->entity2, HEALTH);
    RigidbodyComponent* body1 = ECS_GetComponent(collision->entity1, BODY);
    RigidbodyComponent* body2 = ECS_GetComponent(collision->entity2, BODY);
    PositionComponent* pos1 = ECS_GetComponent(collision->entity1, POSITION);
    PositionComponent* pos2 = ECS_GetComponent(collision->entity2, POSITION);

    ScriptComponent* script1 = ECS_GetComponent(collision->entity1, SCRIPT);

    // Gérer les dégâts et le knockback dans les deux sens
    if (damager2 && health1 && body1 && pos1 && pos2 && script1) {
        PlayerData* playerData = (PlayerData*)script1->data;
        if (playerData && damage(collision->entity1, damager2->damage)) {
            // Direction du knockback
            float dx = pos1->x - pos2->x;
            float dy = pos1->y - pos2->y;
            float len = sqrt(dx*dx + dy*dy);
            if (len > 0) {
                dx /= len;
                dy /= len;
                
                float* knockbackArgs = malloc(sizeof(float) * 3);
                knockbackArgs[0] = dx * 5.0f;  // Vitesse de knockback réduite
                knockbackArgs[1] = dy * 5.0f;
                knockbackArgs[2] = 200.0f;    // Force de knockback augmentée
                
                Force* knockback = create_force(knockback_force, knockbackArgs);
                add_force(collision->entity1, knockback);
                
                printf("Applied knockback to entity %d\n", collision->entity1);
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