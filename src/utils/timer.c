#include "timer.h"

Timer* create_timer(void (*on_end)(void* elt), void* elt) {
    Timer* timer = malloc(sizeof(Timer));
    timer->time = -1;
    timer->start = malloc(sizeof(struct timeval));
    gettimeofday(timer->start, NULL);
    timer->running = false;

    timer->on_end = on_end;
    timer->elt = elt;

    return timer;
}
void free_timer(Timer* timer) {
    free(timer->start);
    free(timer);
}

float get_current_time(Timer* timer) {
    struct timeval now;
    gettimeofday(&now, NULL);
    return (now.tv_sec - timer->start->tv_sec) + (now.tv_usec - timer->start->tv_usec) * 1e-6;
}

void play_timer(Timer* timer, float time) {
    timer->time = time;
    gettimeofday(timer->start, NULL);
    timer->running = true;
}

void update_timer(Timer* timer) {
    if(!timer->running) return;
    if(get_current_time(timer) >= timer->time) {
        timer->running = false;
        timer->on_end(timer->elt);
    }
}