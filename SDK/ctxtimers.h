#ifndef CTXTIMERS_H
#define CTXTIMERS_H

#include <stdint.h>

typedef struct {
    uint64_t start;
    uint64_t duration;
    int active;
} ctx_timer_t;

void ctx_timer_init(ctx_timer_t *timer, uint64_t duration_ms);
void ctx_timer_start(ctx_timer_t *timer);
void ctx_timer_stop(ctx_timer_t *timer);
int ctx_timer_expired(const ctx_timer_t *timer);
uint64_t ctx_timer_elapsed(const ctx_timer_t *timer);

#endif
