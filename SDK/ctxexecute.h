#ifndef CTXEXECUTE_H
#define CTXEXECUTE_H

int ctx_execute(const char *path);
int ctx_execute_args(const char *path, char *const argv[]);
int ctx_execute_wait(const char *path, char *const argv[]);

#endif
