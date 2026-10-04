# 🌐 FreeCTX64 Web Server SDK

![FreeCTX64](https://img.shields.io/badge/FreeCTX64-Web%20Server-0A0A0A?style=for-the-badge)
![C](https://img.shields.io/badge/C-Standard%20C-blue?style=for-the-badge)
![ARM](https://img.shields.io/badge/Architecture-ARM-orange?style=for-the-badge)
![ARM64](https://img.shields.io/badge/ARM64-AArch64-green?style=for-the-badge)
![Linux](https://img.shields.io/badge/Linux-Sockets-yellow?style=for-the-badge)
![No Dependencies](https://img.shields.io/badge/Dependencies-None-purple?style=for-the-badge)

## 🚀 Overview

The **FreeCTX64 Web Server SDK** is a lightweight C interface for serving custom HTML through the FreeCTX64 runtime.

It provides a simple API for configuring a port and supplying HTML content while using the precompiled `ctxweb` runtime inside the FreeCTX64 initramfs.

🌐 Custom HTML  
⚡ Lightweight runtime  
🔌 TCP and HTTP networking  
🧩 Simple C API  
🛠️ Designed for embedded development  
🧠 Suitable for ARM-based systems  

## 🏗️ Architecture

```text
Application
     │
     │ #include "ctxwebserver.h"
     ▼
FreeCTX64 Web Server SDK
     │
     │ configuration + HTML
     ▼
/SDK/ctxweb
     │
     │ TCP socket
     ▼
HTTP Server
     │
     ▼
🌐 Web Browser
```

## 📦 SDK Layout

```text
SDK/
└── ctxwebserver.h

initramfs/
└── SDK/
    └── ctxweb
```

`ctxwebserver.h` provides the developer API.

`ctxweb` is the precompiled runtime responsible for serving the supplied HTML.

## 💻 Basic Usage

```c
#include "ctxwebserver.h"

int main(void)
{
    ctx_webserver_t ctx;

    ctx_webserver_init(&ctx);

    ctx.listen_at_port = 8080;

    ctx.htmlfile =
        "<!DOCTYPE html>"
        "<html>"
        "<head>"
        "<title>My ARM Device</title>"
        "</head>"
        "<body>"
        "<h1>Hello from my device!</h1>"
        "<p>FreeCTX64 Web Server</p>"
        "</body>"
        "</html>";

    return ctx_webserver_start(&ctx);
}
```

## 🌐 Custom HTML

The SDK does **not** require a predefined webpage.

Applications provide their own HTML:

```c
ctx.htmlfile =
    "<html>"
    "<body>"
    "<h1>Custom Website</h1>"
    "<p>Embedded ARM web server.</p>"
    "</body>"
    "</html>";
```

The supplied HTML is passed to the FreeCTX64 `ctxweb` runtime and served directly to HTTP clients.

## 🔌 Port Configuration

```c
ctx.listen_at_port = 8080;
```

Or:

```c
ctx_webserver_set_port(&ctx, 8080);
```

Any valid TCP port from `1` through `65535` can be configured.

## 🧩 HTML Configuration

```c
ctx_webserver_set_html(
    &ctx,
    "<html><body><h1>Hello!</h1></body></html>"
);
```

Retrieve the configured HTML with:

```c
const char *html =
    ctx_webserver_get_html(&ctx);
```

## ⚙️ Runtime

The runtime executable is:

```text
/SDK/ctxweb
```

It accepts the configured port and generated HTML file from the SDK interface.

The runtime uses standard Linux networking interfaces and does not require a third-party HTTP library.

## 🪶 Lightweight Design

The server is intentionally small.

It uses:

- C
- Linux TCP sockets
- POSIX file operations
- Standard C library functionality
- FreeCTX64 initramfs runtime

No web framework is required.

No external HTTP server is required.

No JavaScript runtime is required.

No large userspace stack is required.

## 🤖 ARM-Based Development

FreeCTX64 is designed around **ARM-based development environments**.

The SDK can be used on:

- 🟢 ARM64
- 🟢 AArch64
- 🟢 ARM Linux development systems
- 🟢 ARM single-board computers
- 🟢 Embedded Linux systems
- 🟢 ARM development boards
- 🟢 ARM-based experimental operating systems

The API is written in portable C, while the current FreeCTX64 runtime targets ARM64 Linux.

## 🔧 Microcontroller-Oriented Development

The SDK is also designed with **microcontroller-style development goals** in mind.

For systems capable of running an appropriate C environment and TCP/IP stack, the API can serve as a lightweight model for embedded web interfaces.

Typical uses include:

- 📟 Device configuration pages
- 🌡️ Sensor dashboards
- ⚙️ Hardware control panels
- 📊 Embedded status pages
- 🔧 Development interfaces
- 🖥️ Local device administration
- 📡 Network-enabled embedded systems

On systems without Linux, the socket and runtime layer can be replaced with the platform's native networking implementation while preserving the general SDK concept.

## 🧠 Embedded Philosophy

FreeCTX64 focuses on keeping the web interface close to the application.

```text
Application
   ↓
HTML
   ↓
SDK
   ↓
Runtime
   ↓
TCP
   ↓
Browser
```

This makes it possible to build small development interfaces without requiring a complete web framework.

## 📋 API

### `ctx_webserver_init`

Initializes a server configuration.

```c
void ctx_webserver_init(ctx_webserver_t *ctx);
```

### `ctx_webserver_start`

Starts the FreeCTX64 web-server runtime.

```c
int ctx_webserver_start(ctx_webserver_t *ctx);
```

### `ctx_webserver_set_html`

Sets custom HTML content.

```c
int ctx_webserver_set_html(
    ctx_webserver_t *ctx,
    const char *html
);
```

### `ctx_webserver_set_port`

Sets the listening port.

```c
int ctx_webserver_set_port(
    ctx_webserver_t *ctx,
    int port
);
```

### `ctx_webserver_get_port`

Returns the configured port.

```c
int ctx_webserver_get_port(
    const ctx_webserver_t *ctx
);
```

### `ctx_webserver_get_html`

Returns the configured HTML content.

```c
const char *ctx_webserver_get_html(
    const ctx_webserver_t *ctx
);
```

### `ctx_webserver_stop`

Stops the web-server interface.

```c
void ctx_webserver_stop(
    ctx_webserver_t *ctx
);
```

## 🛠️ Building Applications

A FreeCTX64 application can include the SDK header:

```bash
gcc -std=c11 -O2 -Wall -Wextra -Wpedantic \
    application.c \
    -I/path/to/FreeCTX64/SDK \
    -o application
```

The application uses the SDK interface while the FreeCTX64 runtime provides `ctxweb`.

## 🔒 Networking

The runtime creates a TCP listening socket and provides HTTP responses containing the application's supplied HTML.

The server currently provides a simple HTTP interface intended primarily for embedded and development environments.

## 📱 ARM Device Example

```text
┌──────────────────────────────┐
│       ARM Development        │
│           System             │
├──────────────────────────────┤
│        FreeCTX64              │
│                              │
│  ┌────────────────────────┐  │
│  │     Your Application    │  │
│  └───────────┬────────────┘  │
│              │               │
│              ▼               │
│     ctxwebserver.h           │
│              │               │
│              ▼               │
│        /SDK/ctxweb            │
│              │               │
│              ▼               │
│        TCP Port 8080         │
└──────────────┬───────────────┘
               │
               ▼
          🌐 Browser
```

## 📜 License

FreeCTX64 Web Server SDK is distributed as part of FreeCTX64 under its project license.

---

# 🌐 FreeCTX64 Web Server SDK

### ⚡ Small API • 🧩 Custom HTML • 🔌 TCP • 🤖 ARM • 🛠️ Embedded Development

```text
Free Coding To System 64
```
