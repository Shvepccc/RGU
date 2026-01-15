SRC_DIR := src
INC_DIR := include
BUILD_DIR := build
CC := clang

# Добавляем пути к GMP для macOS
GMP_INC := -I/opt/homebrew/include
GMP_LIB := -L/opt/homebrew/lib -lgmp

# Базовые флаги
CFLAGS := -Wall -Wextra -O2 $(GMP_INC)
LDFLAGS := $(GMP_LIB)

DEBUG_CFLAGS := -Wall -Wextra -g -O0 $(GMP_INC)
DEBUGGER_CMD := lldb
ARGS := # Arguments to pass to run/valgrind

ifeq ($(V),1)
	Q :=
else
	Q := @
endif

TASK ?=

.PHONY: all clean run debug valgrind help

all:
	$(Q)echo "Nothing to build. Use 'make run TASK=<n>' to compile and run src/<n>.c"

# Исправляем правило компиляции - добавляем LDFLAGS
$(BUILD_DIR)/%: $(SRC_DIR)/%.c
	$(Q)echo "Compiling $< -> $@"
	$(Q)echo "CFLAGS: $(CFLAGS)"
	$(Q)mkdir -p $(BUILD_DIR)
	$(Q)$(CC) $(CFLAGS) -I$(INC_DIR) -o $@ $< $(LDFLAGS)

run:
	@if [ -z "$(TASK)" ]; then \
		echo "Error: TASK is not set. Usage: make run TASK=<n>"; \
		exit 1; \
	fi
	$(MAKE) $(BUILD_DIR)/$(TASK)
	$(Q)echo "Running task $(TASK)...\n"
	$(Q)./$(BUILD_DIR)/$(TASK) $(ARGS)

debug: clean
	@if [ -z "$(TASK)" ]; then \
		echo "Error: TASK is not set. Usage: make debug TASK=<n>"; \
		exit 1; \
	fi
	$(Q)echo "Compiling with debug flags..."
	$(MAKE) $(BUILD_DIR)/$(TASK) CFLAGS="$(DEBUG_CFLAGS)" LDFLAGS="$(GMP_LIB)"
	$(Q)echo "\nStarting debug session for task $(TASK)..."
	$(Q)$(DEBUGGER_CMD) ./$(BUILD_DIR)/$(TASK) $(ARGS)

valgrind:
	@if [ -z "$(TASK)" ]; then \
		echo "Error: TASK is not set. Usage: make valgrind TASK=<n>"; \
		exit 1; \
	fi
	$(MAKE) $(BUILD_DIR)/$(TASK)
	$(Q)echo "Running task $(TASK) with Valgrind..."
	$(Q)valgrind --leak-check=full --show-leak-kinds=all \
		--track-origins=yes --error-exitcode=1 \
		./$(BUILD_DIR)/$(TASK) $(ARGS)

clean:
	$(Q)echo "Cleaning build artifacts..."
	$(Q)rm -rf $(BUILD_DIR)

help:
	$(Q)echo "Laboratory Works Build System"
	$(Q)echo "Targets:"
	$(Q)echo "  run TASK=<n>       - Compile and run src/<n>.c"
	$(Q)echo "  debug TASK=<n>       - Debug build/<n> with debugdbg"
	$(Q)echo "  valgrind TASK=<n>  - Run build/<n> under Valgrind"
	$(Q)echo "  clean              - Remove build artifacts"
	$(Q)echo ""
	$(Q)echo "Variables:"
	$(Q)echo "  TASK          - Task number (e.g., 1, 2, ...)"
	$(Q)echo "  V=1           - Verbose output"
	$(Q)echo "  ARGS          - Arguments for run/valgrind"
