#ifndef CTXTSTRAP_H
#define CTXTSTRAP_H

#include <stdint.h>

typedef void (*ctx_tstrap_callback_t)(uint64_t timestamp);

int ctx_tstrap_start(
    uint64_t interval_ms,
    ctx_tstrap_callback_t callback
);

int ctx_tstrap_stop(void);
int ctx_tstrap_active(void);

#endif
