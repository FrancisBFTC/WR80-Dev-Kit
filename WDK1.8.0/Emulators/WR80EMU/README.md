# WR80X Emulator

The **WR80EMU Emulator** is a software emulator for WR80X-based systems, capable of executing WR80X programs and simulating their CPU, memory, and hardware behavior.  
It works alongside the **WR80DBG** Debugger to provide runtime control, inspection, and testing capabilities.

---

## Build Instructions

Before building, make sure you have **GCC** and **GNU Make** installed on your system.

Use the provided **Makefile** to manage the build process.

| Command | Description |
|----------|-------------|
| `make` | Builds the emulator in the `/bin` directory. |
| `make install` | Installs the emulator system-wide. |
| `make update` | Updates from the official WR80 repository and rebuilds. |
| `make clear` | Removes build artifacts and binaries. |
| `make uninstall` | Uninstalls the WR80EMU binary. |

---

## Notes

- The Emulator communicates with the **WR80DBG** Debugger over a local TCP connection to manage program execution and provide CPU state information.
- Internet access is required for version checks and the `make update` command.
- Built binaries are placed inside the `/bin` and `/Tools` directory by default.