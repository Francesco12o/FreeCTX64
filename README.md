# ⚡ FreeCTX64

[![Linux](https://img.shields.io/badge/Linux-Kernel-black?logo=linux)](https://www.kernel.org/)
[![ARM64](https://img.shields.io/badge/Architecture-ARM64-blue)](https://www.arm.com/)
[![C](https://img.shields.io/badge/Language-C-blue?logo=c)](https://en.wikipedia.org/wiki/C_(programming_language))
[![License](https://img.shields.io/badge/License-GPLv3-red)](LICENCE)
[![Status](https://img.shields.io/badge/Status-Development-orange)]()

> 🛠️ **FreeCTX64 — Free Coding To System 64**

A lightweight Linux-based development operating system designed to boot directly into your application.

🚀 **Linux kernel + initramfs + C application = a tiny development system.**

---

## ✨ What is FreeCTX64?

**FreeCTX64** is an operating system based on Linux designed for development.

It includes:

- 🐧 A precompiled Linux kernel
- 📦 A custom initramfs
- 🛠️ Development tools
- ⚙️ A C-based SDK
- 🌐 Web server functionality
- 🧮 Development utilities
- ⏱️ Timers and task support
- 🖥️ Console functionality
- 🚀 Application execution support

The main idea is simple:

```text
Linux Kernel
     │
     ▼
   /init
     │
     ▼
FreeCTX64 Application
     │
     ├── SDK
     ├── Tools
     ├── Tasks
     ├── Timers
     ├── Console
     └── Networking
```

---

# 🧠 What does FreeCTX64 mean?

**FreeCTX64** stands for:

> **Free Coding To System 64**

| Part | Meaning |
|---|---|
| **Free** | Free and open development |
| **C** | Coding |
| **T** | To |
| **X** | System |
| **64** | 64-bit platform |

---

# ⚡ The `/init` philosophy

FreeCTX64 does **not** require a traditional userspace shell to start.

The Linux kernel launches:

```text
/init
```

and `/init` becomes **PID 1**.

That program is the FreeCTX64 application itself.

```text
Linux
  │
  └── /init
        │
        └── Your application
```

This makes FreeCTX64 extremely direct.

No desktop environment.

No unnecessary userspace.

Just:

**Kernel → Application** ⚡

---

# 🛠️ SDK

FreeCTX64 provides a C SDK with headers for application development.

```text
SDK/
├── ctxconsole.h
├── ctxexecute.h
├── ctxmath.h
├── ctxtasks.h
├── ctxtimers.h
├── ctxtstrap.h
└── ctxwebserver.h
```

These APIs provide functionality such as:

- 🖥️ Console output
- ➕ Mathematics
- ⏱️ Timers
- 🧵 Tasks
- 🚀 Process execution
- 🌐 Web servers
- 🧰 System/application utilities

---

# 🌐 Web Server SDK

FreeCTX64 includes a lightweight C web server interface.

Example concept:

```c
#include "ctxwebserver.h"

ctx_webserver_t server;

ctx_webserver_init(&server);
ctx_webserver_set_port(&server, 8080);
ctx_webserver_set_html(&server,
    "<html>"
    "<body>"
    "<h1>Hello from FreeCTX64!</h1>"
    "</body>"
    "</html>"
);

ctx_webserver_start(&server);
```

The application controls the:

- 🌐 Port
- 📄 HTML
- ⚙️ Server configuration

No external web framework is required.

---

# 📦 Runtime

The FreeCTX64 initramfs contains the runtime environment:

```text
initramfs/
├── LIB/
├── SDK/
├── SYS/
│   └── etc/
│       ├── ctx-release
│       └── hostname
├── WS/
└── lib -> LIB/lib
```

The runtime includes utilities such as:

```text
ctx
ctxcat
ctxdel
ctxinfo
ctxls
ctxrun
ctxsh
ctxver
ctxweb
ctxwrite
```

---

# 🐚 ctxsh

`ctxsh` is an optional interactive shell.

It is **not** the FreeCTX64 init process.

You can launch it from an application when interactive development is useful.

```text
FreeCTX64 ctxsh 0.1.0
FreeCTX64 interactive shell
Type 'help' for commands.

ctx>
```

---

# 🔨 Building an application

A FreeCTX64 application can be compiled into:

```text
initramfs/init
```

For example:

```bash
gcc -std=c11 -O2 -Wall -Wextra -Wpedantic \
    main.c \
    -o initramfs/init \
    -pthread
```

Then package the initramfs:

```bash
./TOOLS/initramfs-pack
```

This creates:

```text
boot/initramfs.img
```

---

# 🚀 Booting

FreeCTX64 can be tested with QEMU:

```bash
./TOOLS/ctx-boot
```

The boot chain is:

```text
boot/vmlinuz
      +
boot/initramfs.img
      │
      ▼
   QEMU / ARM64
      │
      ▼
    Linux
      │
      ▼
   /init
      │
      ▼
Your application
```

---

# 🧪 Development workflow

The basic workflow is:

```text
        Write C
           │
           ▼
       Compile
           │
           ▼
      initramfs/init
           │
           ▼
    Package initramfs
           │
           ▼
     initramfs.img
           │
           ▼
       Boot ARM64
           │
           ▼
     Your application
```

🔥 **Write → Build → Pack → Boot**

---

# 🐧 ARM Development

FreeCTX64 is particularly suited to ARM-based development.

The project can be tested on:

- 🖥️ ARM64 virtual machines
- 🥧 Raspberry Pi
- 📦 ARM64 development systems
- 🔧 Embedded Linux environments
- 🧪 QEMU ARM64

The goal is to make application-focused Linux systems extremely small and straightforward.

---

# 🥧 Raspberry Pi

FreeCTX64 can be used with the same basic philosophy on Raspberry Pi:

```text
C source
   │
   ▼
FreeCTX64 build
   │
   ▼
initramfs.img
   │
   ▼
SD card
   │
   ▼
Raspberry Pi
   │
   ▼
Linux kernel
   │
   ▼
/init
   │
   ▼
C application
```

That means a Raspberry Pi can boot directly into a custom C application instead of loading a conventional desktop environment.

🥧⚡ **Tiny Linux appliance.**

---

# 🔌 Microcontroller-oriented philosophy

FreeCTX64 is designed with a similar development philosophy to small embedded platforms:

```text
Small system
     +
Simple application
     +
Direct boot
     +
Hardware
     =
Focused development environment
```

It is **Linux-based**, rather than a traditional microcontroller firmware environment.

This provides access to Linux capabilities while keeping the userspace focused.

---

# 📏 Lightweight

FreeCTX64 intentionally avoids unnecessary software.

The system is centered around:

```text
Linux
+
initramfs
+
C
+
SDK
+
application
```

A small FreeCTX64 system can therefore be only a few megabytes.

⚡ Less userspace.

⚡ Less overhead.

⚡ Faster development.

⚡ Direct application boot.

---

# 🧰 Included tools

The project contains tools for building and booting the system:

```text
TOOLS/
├── ctx-boot
├── ctx-build-clang
├── ctx-build-gcc
└── initramfs-pack
```

### `ctx-build-gcc`

Builds a C application into:

```text
initramfs/init
```

### `initramfs-pack`

Creates:

```text
boot/initramfs.img
```

### `ctx-boot`

Launches the FreeCTX64 ARM64 environment using QEMU.

---

# 🧪 Example application

A FreeCTX64 application can perform normal Linux development tasks:

```c
#include <stdio.h>

int main(void)
{
    printf("Hello from FreeCTX64!\n");
    return 0;
}
```

Compile it:

```bash
gcc -std=c11 -O2 -Wall -Wextra -Wpedantic \
    main.c -o initramfs/init
```

Package:

```bash
./TOOLS/initramfs-pack
```

Boot:

```bash
./TOOLS/ctx-boot
```

And Linux launches your program as PID 1.

🔥 **That's FreeCTX64.**

---

# 📁 Project structure

```text
FreeCTX64/
├── LICENCE
├── README.md
├── SDK/
│   ├── ctxconsole.h
│   ├── ctxexecute.h
│   ├── ctxmath.h
│   ├── ctxtasks.h
│   ├── ctxtimers.h
│   ├── ctxtstrap.h
│   └── ctxwebserver.h
│
├── TOOLS/
│   ├── ctx-boot
│   ├── ctx-build-clang
│   ├── ctx-build-gcc
│   └── initramfs-pack
│
├── boot/
│   └── vmlinuz
│
└── initramfs/
    ├── LIB/
    ├── SDK/
    ├── SYS/
    ├── WS/
    └── lib -> LIB/lib
```

---

# 🔐 Design goals

FreeCTX64 focuses on:

- ⚡ Simplicity
- 🛠️ Development
- 🐧 Linux
- 💻 C programming
- 🥧 ARM systems
- 📦 Small initramfs systems
- 🌐 Lightweight networking
- 🔧 Embedded-oriented development
- 🚀 Direct application boot

---

# 🌟 The idea

Traditional Linux:

```text
Bootloader
   ↓
Kernel
   ↓
Init
   ↓
Userspace
   ↓
Services
   ↓
Shell
   ↓
Application
```

FreeCTX64:

```text
Kernel
   ↓
/init
   ↓
Application
```

**Simple. Direct. Linux.** 🐧⚡

---

# 📜 License

FreeCTX64 is released under the **GNU General Public License v3.0**.

See [`LICENCE`](LICENCE).

---

# ⚡ FreeCTX64

**Free Coding To System 64**

> 🐧 Linux at the core.  
> ⚙️ C at the center.  
> 🚀 Your application at boot.

**Build it. Pack it. Boot it.** 🔥
