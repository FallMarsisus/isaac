#include "timer.h"
#include <stdlib.h>

static Timer* active_timers = NULL;

void init_timer_system(void) {
    active_timers = NULL;
}

void shutdown_timer_system(void) {
    Timer* current = active_timers;
    while (current) {
        Timer* next = current->next;
        free_timer(current);
        current = next;
    }
    active_timers = NULL;
}

void update_timer_system(float delta_time) {
    Timer* current = active_timers;
    
    while (current) {
        Timer* next = current->next;
        
        if (current->active && !current->paused) {
            current->elapsed += delta_time;
            current->remaining = current->duration - current->elapsed;
            
            if (current->elapsed >= current->duration) {
                if (current->callback) {
                    current->callback(current, current->user_data);
                }
                cancel_timer(current);
            }
        }
        
        current = next;
    }
}

Timer* create_timer(float duration, TimerCallback callback, void* user_data) {
    Timer* timer = (Timer*)malloc(sizeof(Timer));
    if (!timer) return NULL;
    
    timer->active = false;
    timer->paused = false;
    timer->duration = duration;
    timer->elapsed = 0.0f;
    timer->remaining = duration;
    timer->callback = callback;
    timer->user_data = user_data;
    timer->next = NULL;
    timer->prev = NULL;
    
    return timer;
}

void free_timer(Timer* timer) {
    if (!timer) return;
    cancel_timer(timer);
    free(timer);
}

void start_timer(Timer* timer) {
    if (!timer || timer->active) return;
    
    timer->active = true;
    timer->paused = false;
    timer->elapsed = 0.0f;
    timer->remaining = timer->duration;
    
    // Add to active timers list
    timer->next = active_timers;
    if (active_timers) {
        active_timers->prev = timer;
    }
    active_timers = timer;
}

void pause_timer(Timer* timer) {
    if (!timer || !timer->active || timer->paused) return;
    timer->paused = true;
}

void resume_timer(Timer* timer) {
    if (!timer || !timer->active || !timer->paused) return;
    timer->paused = false;
}

void cancel_timer(Timer* timer) {
    if (!timer || !timer->active) return;
    
    // Remove from active timers list
    if (timer->prev) {
        timer->prev->next = timer->next;
    } else {
        active_timers = timer->next;
    }
    
    if (timer->next) {
        timer->next->prev = timer->prev;
    }
    
    timer->active = false;
    timer->next = NULL;
    timer->prev = NULL;
}

float get_remaining_timer(const Timer* timer) {
    if (!timer || !timer->active) return 0.0f;
    return timer->remaining;
}

bool is_active_timer(const Timer* timer) {
    return timer && timer->active;
}

bool is_paused_timer(const Timer* timer) {
    return timer && timer->active && timer->paused;
}