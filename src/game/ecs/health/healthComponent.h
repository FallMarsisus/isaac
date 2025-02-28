#pragma once 

typedef struct {
    int health;
    int mana;
    int max_health;
    int max_mana;
    int shield;
    bool isDisplayed;
    Uint32 last_damage_time;    // Temps du dernier dégât reçu
    Uint32 damage_cooldown;     // Durée du cooldown en millisecondes
    Uint32 effect_end_time;     // Nouveau: temps de fin de l'effet visuel
} HealthComponent;