# WR80X Debugger

The **WR80DBG** is the official CLI debugger for the **WR80X architecture**, designed to interact with the **WR80EMU Emulator**.

---

## Build Instructions

Before building, make sure you have **GCC** and **GNU Make** installed on your system.

Use the provided **Makefile** to manage the build process.

| Command | Description |
|----------|-------------|
| `make` | Builds the debugger in the `/bin` directory. |
| `make install` | Installs the debugger system-wide. |
| `make update` | Updates from the official WR80 repository and rebuilds. |
| `make clear` | Removes build artifacts and binaries. |
| `make uninstall` | Uninstalls the WR80DBG binary. |

---

## Notes

- Connects to **WR80EMU** via TCP (`127.0.0.1:8080`). 
- Supports **step-by-step debugging**, **register inspection**, and **breakpoint management**.  
- Built binaries are stored in the `/bin` and `/Tools` directory by default.