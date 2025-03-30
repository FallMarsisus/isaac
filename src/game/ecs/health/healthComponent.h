#pragma once 

typedef struct {
    int health;
    int max_health;

    bool is_invincible;

    int damage_cooldown;
    Uint32 last_damage_time;    // Temps du dernier dégât reçu
} HealthComponent;

typedef struct {
    int damage;
    
    bool damage_player;
} DamagerComponent;