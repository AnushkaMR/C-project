# -------- Compiler & Flags --------
CC      := gcc
CFLAGS  := -Iinclude -Wall -Wextra -O2
LDLIBS  := -lm

SRC_DIR := src
DEMO_DIR:= demo
TEST_DIR:= tests
OBJ_DIR := build/obj
BIN_DIR := build/bin

INCLUDES:= -Iinclude

# Source files and object files
SRCS     := $(wildcard $(SRC_DIR)/*.c)
OBJS     := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

# Demo executables
DEMO_CUBE    := $(BIN_DIR)/demo_cube
DEMO_CYL     := $(BIN_DIR)/demo_cylinder
DEMO_SOCCER  := $(BIN_DIR)/demo_soccer
DEMO_LIGHT   := $(BIN_DIR)/demo_light

.PHONY: all lib demos demo_cube demo_soccer demo_light test clean

# Default target
all: lib demos

# Build static library
lib: $(BIN_DIR)/libtiny3d.a

$(BIN_DIR)/libtiny3d.a: $(OBJS)
	@mkdir -p $(BIN_DIR)
	ar rcs $@ $^

# Compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Build all demos
demos: demo_cube demo_soccer demo_light

demo_cube: lib
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) $(DEMO_DIR)/animate_cube.c \
	      -o $(DEMO_CUBE) $(BIN_DIR)/libtiny3d.a $(LDLIBS)
	@echo "Built: $(DEMO_CUBE)"

demo_soccer: lib
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) $(DEMO_DIR)/animate_soccer.c \
	      -o $(DEMO_SOCCER) $(BIN_DIR)/libtiny3d.a $(LDLIBS)
	@echo "Built: $(DEMO_SOCCER)"

demo_light: lib
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) $(DEMO_DIR)/mainlight.c \
	      -o $(DEMO_LIGHT) $(BIN_DIR)/libtiny3d.a $(LDLIBS)
	@echo "Built: $(DEMO_LIGHT)"

# Build and run each test file individually
test: lib
	@mkdir -p $(BIN_DIR)
	@for file in $(wildcard $(TEST_DIR)/*.c); do \
		name=$$(basename $$file .c); \
		echo "Building and running test: $$name"; \
		$(CC) $(CFLAGS) $(INCLUDES) $$file -o $(BIN_DIR)/test_$$name $(BIN_DIR)/libtiny3d.a $(LDLIBS) && \
		$(BIN_DIR)/test_$$name || exit 1; \
	done

# Clean all build files
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
