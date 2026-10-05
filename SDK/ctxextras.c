#define _POSIX_C_SOURCE 200809L

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/wait.h>

#include "ctxconsole.h"
#include "ctxexecute.h"
#include "ctxmath.h"
#include "ctxtasks.h"
#include "ctxtimers.h"
#include "ctxtstrap.h"

/* ============================================================
 * Console
 * ============================================================ */

int ctx_console_write(const char *text)
{
    if (text == NULL)
        return -1;

    fputs(text, stdout);
    fflush(stdout);

    return 0;
}

int ctx_console_write_n(const char *text, size_t length)
{
    if (text == NULL)
        return -1;

    fwrite(text, 1, length, stdout);
    fflush(stdout);

    return 0;
}

int ctx_console_printf(const char *format, ...)
{
    int result;
    va_list args;

    if (format == NULL)
        return -1;

    va_start(args, format);
    result = vprintf(format, args);
    va_end(args);

    fflush(stdout);

    return result;
}

int ctx_console_read(char *buffer, size_t size)
{
    if (buffer == NULL || size == 0)
        return -1;

    if (fgets(buffer, (int)size, stdin) == NULL)
        return -1;

    return 0;
}

int ctx_console_execute(const char *command)
{
    if (command == NULL)
        return -1;

    return system(command);
}

void ctx_console_clear(void)
{
    fputs("\033[2J\033[H", stdout);
    fflush(stdout);
}

/* ============================================================
 * Execution
 * ============================================================ */

int ctx_execute(const char *path)
{
    char *const argv[] = {
        (char *)path,
        NULL
    };

    return ctx_execute_args(path, argv);
}

int ctx_execute_args(const char *path, char *const argv[])
{
    pid_t pid;

    if (path == NULL || argv == NULL)
        return -1;

    pid = fork();

    if (pid < 0)
        return -1;

    if (pid == 0) {
        execv(path, argv);
        _exit(127);
    }

    return 0;
}

int ctx_execute_wait(const char *path, char *const argv[])
{
    pid_t pid;
    int status;

    if (path == NULL || argv == NULL)
        return -1;

    pid = fork();

    if (pid < 0)
        return -1;

    if (pid == 0) {
        execv(path, argv);
        _exit(127);
    }

    if (waitpid(pid, &status, 0) < 0)
        return -1;

    if (WIFEXITED(status))
        return WEXITSTATUS(status);

    return -1;
}

/* ============================================================
 * Math
 * ============================================================ */

int64_t ctxmath_add_i64(int64_t a, int64_t b)
{
    return a + b;
}

int64_t ctxmath_sub_i64(int64_t a, int64_t b)
{
    return a - b;
}

int64_t ctxmath_mul_i64(int64_t a, int64_t b)
{
    return a * b;
}

int64_t ctxmath_div_i64(int64_t a, int64_t b)
{
    if (b == 0)
        return 0;

    return a / b;
}

uint64_t ctxmath_add_u64(uint64_t a, uint64_t b)
{
    return a + b;
}

uint64_t ctxmath_sub_u64(uint64_t a, uint64_t b)
{
    return a - b;
}

uint64_t ctxmath_mul_u64(uint64_t a, uint64_t b)
{
    return a * b;
}

uint64_t ctxmath_div_u64(uint64_t a, uint64_t b)
{
    if (b == 0)
        return 0;

    return a / b;
}

int ctxmath_abs_i64(int64_t value)
{
    if (value < 0)
        value = -value;

    return (int)value;
}

/* ============================================================
 * Tasks
 * ============================================================ */

typedef struct {
    ctx_task_function_t function;
    void *argument;
    pthread_t thread;
    int started;
    int finished;
} ctx_task_internal_t;

#define CTX_MAX_TASKS 64

static ctx_task_internal_t task_table[CTX_MAX_TASKS];
static pthread_mutex_t task_lock = PTHREAD_MUTEX_INITIALIZER;

static void *ctx_task_entry(void *argument)
{
    ctx_task_internal_t *task =
        (ctx_task_internal_t *)argument;

    task->function(task->argument);

    pthread_mutex_lock(&task_lock);
    task->finished = 1;
    pthread_mutex_unlock(&task_lock);

    return NULL;
}

ctx_task_id_t ctx_task_create(
    ctx_task_function_t function,
    void *argument
)
{
    int i;

    if (function == NULL)
        return -1;

    pthread_mutex_lock(&task_lock);

    for (i = 0; i < CTX_MAX_TASKS; ++i) {
        if (task_table[i].function == NULL) {
            task_table[i].function = function;
            task_table[i].argument = argument;
            task_table[i].started = 0;
            task_table[i].finished = 0;

            pthread_mutex_unlock(&task_lock);

            return i;
        }
    }

    pthread_mutex_unlock(&task_lock);

    return -1;
}

int ctx_task_start(ctx_task_id_t task)
{
    if (task < 0 || task >= CTX_MAX_TASKS)
        return -1;

    pthread_mutex_lock(&task_lock);

    if (task_table[task].function == NULL ||
        task_table[task].started) {
        pthread_mutex_unlock(&task_lock);
        return -1;
    }

    task_table[task].started = 1;

    if (pthread_create(
            &task_table[task].thread,
            NULL,
            ctx_task_entry,
            &task_table[task]) != 0) {

        task_table[task].started = 0;

        pthread_mutex_unlock(&task_lock);

        return -1;
    }

    pthread_mutex_unlock(&task_lock);

    return 0;
}

int ctx_task_stop(ctx_task_id_t task)
{
    if (task < 0 || task >= CTX_MAX_TASKS)
        return -1;

    pthread_mutex_lock(&task_lock);

    if (!task_table[task].started) {
        pthread_mutex_unlock(&task_lock);
        return -1;
    }

    pthread_cancel(task_table[task].thread);

    pthread_mutex_unlock(&task_lock);

    return 0;
}

int ctx_task_wait(ctx_task_id_t task)
{
    if (task < 0 || task >= CTX_MAX_TASKS)
        return -1;

    pthread_mutex_lock(&task_lock);

    if (!task_table[task].started) {
        pthread_mutex_unlock(&task_lock);
        return -1;
    }

    pthread_t thread = task_table[task].thread;

    pthread_mutex_unlock(&task_lock);

    if (pthread_join(thread, NULL) != 0)
        return -1;

    return 0;
}

int ctx_task_running(ctx_task_id_t task)
{
    int running;

    if (task < 0 || task >= CTX_MAX_TASKS)
        return 0;

    pthread_mutex_lock(&task_lock);

    running =
        task_table[task].started &&
        !task_table[task].finished;

    pthread_mutex_unlock(&task_lock);

    return running;
}

/* ============================================================
 * Timers
 * ============================================================ */

static uint64_t ctx_now_ms(void)
{
    struct timespec ts;

    clock_gettime(CLOCK_MONOTONIC, &ts);

    return
        ((uint64_t)ts.tv_sec * 1000ULL) +
        ((uint64_t)ts.tv_nsec / 1000000ULL);
}

void ctx_timer_init(ctx_timer_t *timer, uint64_t duration_ms)
{
    if (timer == NULL)
        return;

    timer->start = 0;
    timer->duration = duration_ms;
    timer->active = 0;
}

void ctx_timer_start(ctx_timer_t *timer)
{
    if (timer == NULL)
        return;

    timer->start = ctx_now_ms();
    timer->active = 1;
}

void ctx_timer_stop(ctx_timer_t *timer)
{
    if (timer == NULL)
        return;

    timer->active = 0;
}

int ctx_timer_expired(const ctx_timer_t *timer)
{
    uint64_t now;

    if (timer == NULL || !timer->active)
        return 0;

    now = ctx_now_ms();

    return (now - timer->start) >= timer->duration;
}

uint64_t ctx_timer_elapsed(const ctx_timer_t *timer)
{
    uint64_t now;

    if (timer == NULL || !timer->active)
        return 0;

    now = ctx_now_ms();

    return now - timer->start;
}

/* ============================================================
 * Timestrap
 * ============================================================ */

static pthread_t tstrap_thread;
static pthread_mutex_t tstrap_lock = PTHREAD_MUTEX_INITIALIZER;

static uint64_t tstrap_interval = 0;
static ctx_tstrap_callback_t tstrap_callback = NULL;
static int tstrap_running = 0;

static void *tstrap_worker(void *argument)
{
    (void)argument;

    while (1) {
        struct timespec sleep_time;
        uint64_t interval;
        ctx_tstrap_callback_t callback;

        pthread_mutex_lock(&tstrap_lock);

        if (!tstrap_running) {
            pthread_mutex_unlock(&tstrap_lock);
            break;
        }

        interval = tstrap_interval;
        callback = tstrap_callback;

        pthread_mutex_unlock(&tstrap_lock);

        sleep_time.tv_sec =
            (time_t)(interval / 1000ULL);

        sleep_time.tv_nsec =
            (long)((interval % 1000ULL) * 1000000ULL);

        nanosleep(&sleep_time, NULL);

        pthread_mutex_lock(&tstrap_lock);

        if (tstrap_running && callback != NULL) {
            uint64_t timestamp = ctx_now_ms();

            pthread_mutex_unlock(&tstrap_lock);

            callback(timestamp);
        } else {
            pthread_mutex_unlock(&tstrap_lock);
        }
    }

    return NULL;
}

int ctx_tstrap_start(
    uint64_t interval_ms,
    ctx_tstrap_callback_t callback
)
{
    if (interval_ms == 0 || callback == NULL)
        return -1;

    pthread_mutex_lock(&tstrap_lock);

    if (tstrap_running) {
        pthread_mutex_unlock(&tstrap_lock);
        return -1;
    }

    tstrap_interval = interval_ms;
    tstrap_callback = callback;
    tstrap_running = 1;

    if (pthread_create(
            &tstrap_thread,
            NULL,
            tstrap_worker,
            NULL) != 0) {

        tstrap_running = 0;
        tstrap_callback = NULL;

        pthread_mutex_unlock(&tstrap_lock);

        return -1;
    }

    pthread_mutex_unlock(&tstrap_lock);

    return 0;
}

int ctx_tstrap_stop(void)
{
    pthread_mutex_lock(&tstrap_lock);

    if (!tstrap_running) {
        pthread_mutex_unlock(&tstrap_lock);
        return 0;
    }

    tstrap_running = 0;

    pthread_mutex_unlock(&tstrap_lock);

    pthread_join(tstrap_thread, NULL);

    pthread_mutex_lock(&tstrap_lock);

    tstrap_callback = NULL;
    tstrap_interval = 0;

    pthread_mutex_unlock(&tstrap_lock);

    return 0;
}

int ctx_tstrap_active(void)
{
    int active;

    pthread_mutex_lock(&tstrap_lock);
    active = tstrap_running;
    pthread_mutex_unlock(&tstrap_lock);

    return active;
}
