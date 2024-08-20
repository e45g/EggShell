CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99
LIBS = -lreadline

SRC_DIR = src
BIN_DIR = bin
OBJ_DIR = obj

TARGET = $(BIN_DIR)/eggshell
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

.PHONY: all clean directories run

# Change the default target to 'run'
.DEFAULT_GOAL := run

run: all
	@$(TARGET)

all: directories $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)  # Ensure OBJ_DIR exists
	$(CC) $(CFLAGS) -c $< -o $@

directories:
	@mkdir -p $(BIN_DIR)

clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR)

