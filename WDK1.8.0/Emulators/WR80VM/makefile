# WR80-Emulator Makefile

# --- Configuration ---
CC           ?= gcc
CFLAGS       ?= -w -O2 -std=c11 `sdl2-config --cflags`
LIBS 		 ?= `sdl2-config --libs` -lpthread
TARGET       := wr80vm
SRC_DIR      := src
SRC_FILE	 := wr80vm_linux
INC_DIR      := .
BUILD_DIR    := bin
BIN_DIR      := /usr/local/bin
TOOLS_DIR 	 := ../../Tools/WR80/Linux
VERSION_FILE := version.txt

# --- GitHub repo ---
REPO_URL    := https://github.com/FrancisBFTC/WR80-Dev-Kit.git
RAW_URL     := https://raw.githubusercontent.com/FrancisBFTC/WR80-Dev-Kit/wdk1.8.0-compat
SUB_PATH     := WDK1.8.0/Emulators/WR80VM

# --- Sources/Objects ---
SRC := $(wildcard $(SRC_DIR)/$(SRC_FILE).c)
OBJ := $(patsubst $(SRC_DIR)/$(SRC_FILE).c,$(BUILD_DIR)/$(SRC_FILE).o,$(SRC))

# --- Default build ---
default:
	@local_ver=$$(cat $(VERSION_FILE) 2>/dev/null || echo "0.0.0"); \
	echo "Building WR80-Emulator v$$local_ver..."; \
	remote_ver=$$(curl -fsSL "$(RAW_URL)/$(SUB_PATH)/$(VERSION_FILE)" 2>/dev/null || echo "unknown"); \
	if [ "$$remote_ver" = "unknown" ]; then \
		echo "[warning] Could not check remote version."; \
	elif [ "$$remote_ver" = "$$local_ver" ]; then \
		echo "[ok] Version $$local_ver is up to date."; \
	elif [ "$(printf '%s\n%s\n' "$$remote_ver" "$$local_ver" | sort -V | head -n1)" = "$$local_ver" ] && \
	   [ "$$local_ver" != "$$remote_ver" ]; then \
			echo "[warning] A newer version is available: $$remote_ver"; \
	else \
			echo "[warning] Local version $$local_ver is newer. Update the remote version."; \
	fi; \
	mkdir -p $(BUILD_DIR); \
	for file in $(SRC); do \
		obj=$(BUILD_DIR)/$$(basename $$file .c).o; \
		$(CC) $(CFLAGS) -I$(INC_DIR) -c $$file -o $$obj; \
	done; \
	$(CC) $(CFLAGS) -I$(INC_DIR) -o $(BUILD_DIR)/$(TARGET) $(OBJ) $(LDFLAGS) $(LIBS); \
	echo "[ok] Build complete -> $(BUILD_DIR)/$(TARGET)"

# --- Install ---
install:
	@echo "Building and installing $(TARGET)..."
	$(MAKE)
	sudo cp $(BUILD_DIR)/$(TARGET) $(BIN_DIR)/
	sudo cp $(BUILD_DIR)/$(TARGET) $(TOOLS_DIR)/
	@echo "[ok] Installed at $(BIN_DIR)/$(TARGET)"

# --- Uninstall ---
uninstall:
	@echo "Uninstalling $(TARGET)..."
	sudo rm -f $(BIN_DIR)/$(TARGET)
	@echo "[ok] Uninstalled."

# --- Clear build artifacts ---
clear:
	@echo "Cleaning build files..."
	rm -rf $(BUILD_DIR)
	@echo "[ok] Build directory cleared."

# --- Update from GitHub ---
update:
	@echo "Checking for updates..."
	@if [ ! -f $(VERSION_FILE) ]; then echo "0.0.0" > $(VERSION_FILE); fi; \
	current_ver=$$(cat $(VERSION_FILE)); \
	remote_ver=$$(curl -fsSL "$(RAW_URL)/$(SUB_PATH)/$(VERSION_FILE)" 2>/dev/null || echo "unknown"); \
	if [ "$$remote_ver" = "unknown" ]; then \
		echo "[warning] Could not check remote version."; \
	elif [ "$$remote_ver" = "$$current_ver" ]; then \
		echo "[ok] Already up to date (version $$current_ver)."; \
	else \
		echo "[warning] Updating from version $$current_ver to $$remote_ver..."; \
		tmpdir=$$(mktemp -d); \
		git clone -q --depth=1 --branch linux $(REPO_URL) $$tmpdir >/dev/null 2>&1; \
		cp -r $$tmpdir/$(SUB_PATH)/* .; \
		rm -rf $$tmpdir; \
		$(MAKE) clear; \
		$(MAKE); \
		echo "[ok] Update complete (version $$remote_ver)."; \
	fi

build: default
.PHONY: default install uninstall clear update build