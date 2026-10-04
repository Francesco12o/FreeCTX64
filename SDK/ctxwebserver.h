#ifndef CTXWEBSERVER_H
#define CTXWEBSERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#ifdef __cplusplus
extern "C" {
#endif

#define CTX_WEBSERVER_VERSION "0.1.0"
#define CTX_WEBSERVER_RUNTIME "/SDK/ctxweb"
#define CTX_WEBSERVER_DEFAULT_PORT 8080

#define CTX_WEBSERVER_OK       0
#define CTX_WEBSERVER_ERROR   -1
#define CTX_WEBSERVER_INVALID -2

typedef struct
{
    int listen_at_port;
    const char *htmlfile;

} ctx_webserver_t;

static inline void ctx_webserver_init(
    ctx_webserver_t *ctx
)
{
    if (ctx == NULL)
        return;

    ctx->listen_at_port =
        CTX_WEBSERVER_DEFAULT_PORT;

    ctx->htmlfile = NULL;
}

static inline int ctx_webserver_set_html(
    ctx_webserver_t *ctx,
    const char *html
)
{
    if (ctx == NULL || html == NULL)
        return CTX_WEBSERVER_INVALID;

    ctx->htmlfile = html;

    return CTX_WEBSERVER_OK;
}

static inline int ctx_webserver_set_port(
    ctx_webserver_t *ctx,
    int port
)
{
    if (ctx == NULL ||
        port < 1 ||
        port > 65535)
        return CTX_WEBSERVER_INVALID;

    ctx->listen_at_port = port;

    return CTX_WEBSERVER_OK;
}

static inline int ctx_webserver_get_port(
    const ctx_webserver_t *ctx
)
{
    if (ctx == NULL)
        return -1;

    return ctx->listen_at_port;
}

static inline const char *ctx_webserver_get_html(
    const ctx_webserver_t *ctx
)
{
    if (ctx == NULL)
        return NULL;

    return ctx->htmlfile;
}

static inline int ctx_webserver_start(
    ctx_webserver_t *ctx
)
{
    char path[128];
    char port[16];

    int fd;

    if (ctx == NULL ||
        ctx->htmlfile == NULL)
        return CTX_WEBSERVER_INVALID;

    if (ctx->listen_at_port < 1 ||
        ctx->listen_at_port > 65535)
        return CTX_WEBSERVER_INVALID;

    snprintf(
        path,
        sizeof(path),
        "/tmp/ctxweb-%ld.html",
        (long)getpid()
    );

    fd = open(
        path,
        O_WRONLY |
        O_CREAT |
        O_TRUNC,
        0600
    );

    if (fd < 0)
        return CTX_WEBSERVER_ERROR;

    {
        size_t length =
            strlen(ctx->htmlfile);

        size_t written = 0;

        while (written < length) {

            ssize_t result = write(
                fd,
                ctx->htmlfile + written,
                length - written
            );

            if (result <= 0) {
                close(fd);
                unlink(path);
                return CTX_WEBSERVER_ERROR;
            }

            written += (size_t)result;
        }
    }

    close(fd);

    snprintf(
        port,
        sizeof(port),
        "%d",
        ctx->listen_at_port
    );

    execl(
        CTX_WEBSERVER_RUNTIME,
        CTX_WEBSERVER_RUNTIME,
        "--port",
        port,
        "--html-file",
        path,
        (char *)NULL
    );

    unlink(path);

    return CTX_WEBSERVER_ERROR;
}

static inline void ctx_webserver_stop(
    ctx_webserver_t *ctx
)
{
    (void)ctx;
}

#ifdef __cplusplus
}
#endif

#endif
