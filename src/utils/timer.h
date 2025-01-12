#pragma once

#include <sys/time.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    bool running;
    float maxTime;

    void* elt;

    struct timeval* start;
    void (*on_end)(void* elt);
} Timer;

Timer* create_timer(float time, void (*on_end)(void* elt), void* elt);
float get_current_time(Timer* timer);

void play_timer(Timer* timer);
void update_timer(Timer* timer);