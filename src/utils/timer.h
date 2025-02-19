#pragma once

#include <SDL2/SDL.h>
#include <stdbool.h>

typedef struct timer_s Timer;

typedef void (*TimerCallback)(Timer* timer, void* user_data);

struct timer_s {
    bool active;
    bool paused;
    float duration;
    float elapsed;
    float remaining;
    
    TimerCallback callback;
    void* user_data;
    
    // For linked list of active timers
    struct timer_s* next;
    struct timer_s* prev;
};

// Timer system functions
void init_timer_system(void);
void shutdown_timer_system(void);
void update_timer_system(float delta_time);

// Timer functions
Timer* create_timer(float duration, TimerCallback callback, void* user_data);
void free_timer(Timer* timer);
void start_timer(Timer* timer);
void pause_timer(Timer* timer);
void resume_timer(Timer* timer);
void cancel_timer(Timer* timer);
float get_remaining_timer(const Timer* timer);
bool is_active_timer(const Timer* timer);
bool is_paused_timer(const Timer* timer);