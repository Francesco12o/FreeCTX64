#ifndef CTXTASKS_H
#define CTXTASKS_H

typedef int ctx_task_id_t;

typedef void (*ctx_task_function_t)(void *argument);

ctx_task_id_t ctx_task_create(
    ctx_task_function_t function,
    void *argument
);

int ctx_task_start(ctx_task_id_t task);
int ctx_task_stop(ctx_task_id_t task);
int ctx_task_wait(ctx_task_id_t task);
int ctx_task_running(ctx_task_id_t task);

#endif
