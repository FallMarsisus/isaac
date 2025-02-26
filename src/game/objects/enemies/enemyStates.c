#include "enemyStates.h"

#define RAYCAST_STEP 32.0f
#define RAYCAST_RADIUS 16.0f

#define ATTACK_RANGE 300
#define ABANDON_RANGE 600
#define CHASE_OFFSET_RANGE 0.1f
#define FRAME_TIME (1.0f/60.0f)

//Returns False if touch something
bool raycast(Vector start, Vector dir, float distance) {
    normalize(&dir);

    Vector pos = start;
    int steps = distance / RAYCAST_STEP;

    pos.x += dir.x * RAYCAST_STEP;
    pos.y += dir.y * RAYCAST_STEP;
    for (int i = 0; i < steps; i++) {
        pos.x += dir.x * RAYCAST_STEP;
        pos.y += dir.y * RAYCAST_STEP;

        for (Entity e = ECS_GetFirstEntity(); e != -1; e = ECS_GetNextEntity(e)) {
            PositionComponent* ePos = ECS_GetComponent(e, POSITION);
            RigidbodyComponent* eRb = ECS_GetComponent(e, BODY);
            if(!ePos || !eRb || eRb->is_dynamic || fabs(ePos->x - pos.x) > 2 * distance || fabs(ePos->y - pos.y) > 2 * distance)
                continue;

            if(checkCircleCollision(ePos, eRb, pos.x, pos.y, RAYCAST_RADIUS)) return false;
        }
    }
    return true;
}

void randomize_idle_vars(IdleStateVars* vars) {
    vars->direction = (Vector) {
        random_float(-1, 1),
        random_float(-1, 1)
    };
    normalize(&vars->direction);
    vars->speed = random_float(0.4, 1.2);
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
    if(vector_size(&dir) < ATTACK_RANGE && raycast(pos, dir, vector_size(&dir))) {
        StateChangeEvent* event = malloc(sizeof(StateChangeEvent));
        event->id = id; event->new_state = "chase";
        trigger_event(EVENT_STATE_CHANGE, event, true);
        return;
    }

    if(vars->wander_time > 0) {
        vars->wander_time -= FRAME_TIME;

        normalize(&vars->direction);
        posComp->vx = vars->direction.x * vars->speed;
        posComp->vy = vars->direction.y * vars->speed;
    }
    else {
        randomize_idle_vars(vars);
    }

    if(anim) {
        if(vars->direction.y < -0.5) set_active_anim(anim, 1);
        else if(vars->direction.y > 0.5) set_active_anim(anim, 0);
        else if(vars->direction.x < -0.5) set_active_anim(anim, 2);
        else if(vars->direction.x > 0.5) set_active_anim(anim, 3);

        play_anim(anim);
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
    vars->speed = random_float(1.5, 2.5);
    return vars;
}
void on_chase_enter(State* state, uint32_t id) {
    ChaseStateVars* vars = (ChaseStateVars*) state->vars;
    if(!vars) return;
}
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
    Vector targetPos = {
        targetPosComp->x + targetSpriteComp->width / 2, 
        targetPosComp->y + targetSpriteComp->height / 2
    };
    
    Vector dir = (Vector) {
        targetPos.x - pos.x,
        targetPos.y - pos.y
    };
    if(2 * RAYCAST_RADIUS < vector_size(&dir) && vector_size(&dir) < ATTACK_RANGE && !raycast(pos, dir, vector_size(&dir))) {
        StateChangeEvent* event = malloc(sizeof(StateChangeEvent));
        event->id = id; event->new_state = "follow";
        trigger_event(EVENT_STATE_CHANGE, event, true);
        return;
    }
    if(vector_size(&dir) > ABANDON_RANGE) {
        StateChangeEvent* event = malloc(sizeof(StateChangeEvent));
        event->id = id; event->new_state = "idle";
        trigger_event(EVENT_STATE_CHANGE, event, true);
        return;
    }

    normalize(&dir);
    posComp->vx = (dir.x + random_float(-CHASE_OFFSET_RANGE, CHASE_OFFSET_RANGE)) * vars->speed;
    posComp->vy = (dir.y + random_float(-CHASE_OFFSET_RANGE, CHASE_OFFSET_RANGE)) * vars->speed;

    if(anim) {
        if(dir.y < -0.5) set_active_anim(anim, 1);
        else if(dir.y > 0.5) set_active_anim(anim, 0);
        else if(dir.x < -0.5) set_active_anim(anim, 2);
        else if(dir.x > 0.5) set_active_anim(anim, 3);
        play_anim(anim);
    }
}
void on_chase_exit(State* state, uint32_t id) {}
void on_chase_free(State* state, uint32_t id) {
    ChaseStateVars* vars = (ChaseStateVars*) state->vars;
    if(vars) free(vars);
}

Queue* player_positions;
int prev_player_pos_update;

int nb_player_positions = 20;

void init_player_positions(uint32_t id) {
    player_positions = create_queue();
    prev_player_pos_update = SDL_GetTicks();

    PositionComponent* pos = ECS_GetComponent(id, POSITION);
    SpriteComponent* sprite = ECS_GetComponent(id, SPRITE);
    if(pos) {
        Vector player_pos = (Vector) {pos->x + sprite->width / 2, pos->y + sprite->height / 2};
        for(int i = 0; i < nb_player_positions; i++) {
            queue_enqueue(player_positions, &player_pos, sizeof(Vector));
        }
    }
}
void update_player_positions(uint32_t id) {
    if(SDL_GetTicks() - prev_player_pos_update > 200) {
        prev_player_pos_update = SDL_GetTicks();

        Vector last_player_pos = {0, 0};

        PositionComponent* pos = ECS_GetComponent(id, POSITION);
        SpriteComponent* sprite = ECS_GetComponent(id, SPRITE);
        if(pos && sprite) {
            if (queue_size(player_positions) > nb_player_positions) {
                queue_dequeue(player_positions, &last_player_pos, sizeof(Vector));
            }
            
            Vector player_pos = (Vector) {pos->x + sprite->width / 2, pos->y + sprite->height / 2};
            queue_enqueue(player_positions, &player_pos, sizeof(Vector));
        }
    }
}

FollowStateVars* create_follow_vars(uint32_t target) {
    FollowStateVars* vars = malloc(sizeof(FollowStateVars));
    vars->target = target;
    vars->speed = random_float(1.5, 2.5);
    vars->currentGoal = NULL;

    return vars;
}
void on_follow_enter(State* state, uint32_t id) {
    FollowStateVars* vars = (FollowStateVars*) state->vars;
    if(!vars) return;

    PositionComponent* pos = ECS_GetComponent(id, POSITION);
    PositionComponent* targetPos = ECS_GetComponent(vars->target, POSITION);
    SpriteComponent* sprite = ECS_GetComponent(vars->target, SPRITE);
    if(!pos || !targetPos || !sprite) return;

    if(queue_size(player_positions) == 0) {
        Vector player_pos = (Vector) {pos->x + sprite->width / 2, pos->y + sprite->height / 2};
        queue_enqueue(player_positions, &player_pos, sizeof(Vector));
    }

    QueueNode* closest = get_first_queue_node(player_positions);
    Vector closest_pos = *(Vector*) get_data_queue_node(closest);
    Vector dirToClosest = (Vector) {closest_pos.x - pos->x, closest_pos.y - pos->y};

    for(QueueNode* node = get_first_queue_node(player_positions); node; node = get_next_queue_node(node)) {
        Vector player_pos = *(Vector*) get_data_queue_node(node);
        Vector dir = (Vector) {player_pos.x - pos->x, player_pos.y - pos->y};
        if(vector_size(&dir) < vector_size(&dirToClosest)) {
            closest = node;
            dirToClosest = dir;
        }
    }

    vars->currentGoal = closest;
}
void on_follow_update(State* state, uint32_t id) {
    FollowStateVars* vars = (FollowStateVars*) state->vars;
    if(!vars) return;
    
    if(vars->currentGoal == get_first_queue_node(player_positions)) {
        vars->currentGoal = get_next_queue_node(vars->currentGoal);
    }

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
    if(vector_size(&dirToTarget) < ATTACK_RANGE && raycast(pos, dirToTarget, vector_size(&dirToTarget))) {
        StateChangeEvent* event = malloc(sizeof(StateChangeEvent));
        event->id = id; event->new_state = "chase";
        trigger_event(EVENT_STATE_CHANGE, event, true);
        return;
    }
    if(vector_size(&dirToTarget) > ABANDON_RANGE) {
        StateChangeEvent* event = malloc(sizeof(StateChangeEvent));
        event->id = id; event->new_state = "idle";
        trigger_event(EVENT_STATE_CHANGE, event, true);
        return;
    }
    
    Vector* goal_data = get_data_queue_node(vars->currentGoal);
    Vector goal_pos = *goal_data;

    if(checkCircleCollision(posComp, rb, goal_pos.x, goal_pos.y, 16)) {
        QueueNode* next = get_next_queue_node(vars->currentGoal);
        if (next && get_data_queue_node(next)) {
            vars->currentGoal = next;
        } else {
            // If we've reached the end of the queue or next is invalid, start from the beginning
            vars->currentGoal = get_next_queue_node(get_first_queue_node(player_positions));
        }
    }

    Vector dir = {goal_pos.x - pos.x, goal_pos.y - pos.y};
    if(vector_size(&dir) > 0.1) {
        normalize(&dir);
        posComp->vx = dir.x * vars->speed;
        posComp->vy = dir.y * vars->speed;

        if(anim) {
            if(dir.y < -0.5) set_active_anim(anim, 1);
            else if(dir.y > 0.5) set_active_anim(anim, 0);
            else if(dir.x < -0.5) set_active_anim(anim, 2);
            else if(dir.x > 0.5) set_active_anim(anim, 3);
            play_anim(anim);
        }
    }
}
void on_follow_exit(State* state, uint32_t id) {
    FollowStateVars* vars = (FollowStateVars*) state->vars;
    if(!vars) return;
}
void on_follow_free(State* state, uint32_t id) {
    FollowStateVars* vars = (FollowStateVars*) state->vars;
    if(vars) free(vars);
}