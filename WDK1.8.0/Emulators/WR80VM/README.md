# WR80X Virtual Machine

The **WR80X Virtual Machine (WR80VM)** is a graphical runtime environment for WR80X executables.  
It provides a visual 320×200x256 VGA-like display and executes WR80X binaries using the **WR80EMU** core.

---

## Build Instructions

Before building, ensure you have a compatible **C compiler** (such as GCC), **GNU Make**, and **SDL2 development libraries** installed on your system.

Use the provided **Makefile** to build, install, and manage the Virtual Machine.

| Command | Description |
|----------|--------------|
| `make` | Builds the Virtual Machine in the `/bin` directory. |
| `make install` | Builds and installs the Virtual Machine system-wide, allowing it to be executed from any directory. |
| `make update` | Checks for updates in the WR80 development kit repository and rebuilds the tool if a newer version is available. |
| `make clear` | Removes all previously generated binaries and build artifacts. |
| `make uninstall` | Uninstalls the WR80VM from the system. |

---

## Notes

- The WR80VM uses **SDL2** for rendering and runs the emulator core (`WR80EMU`) internally.
	- e. g. Run **sudo apt install libsdl2-dev**
- Internet access is required for version checks and the `make update` command.
- Built binaries are placed inside the `/bin` and `/Tools` directory by default