# A Makefile automates compilation so you only recompile source files that changed.

# Compiler and Flags
CC		:= gcc
CFLAGS  := -Wall -Wexstra -std=c99 -Iinclude
TARGET  := app

# Directories
SRC_DIR 	:= src
BUILD_DIR	:= build

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/*.c, $(BUILD_DIR)/%.o, $(SRCS))

# Default target
all: $(TARGET)

# Link object files into final executable
$(TARGET) : $(OBJS)
		@mkdir -p $(dir $@)
		$(CC) $(OBJS) -o $@
		@echo "Build complete: ./$(TARGET)"

# Compile source files into object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
		@mkdir -p $(BUILD_DIR)
		$(CC) $(CFLAGS) -c $< -o $@
	
# Clean build directory and excutable
clean:
		rm -rf $(BUILD_DIR) $(TARGET)
		@echo "Clean complete."

.PHONY: all clean
