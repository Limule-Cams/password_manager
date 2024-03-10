CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
SRC_DIR = src
BUILD_DIR = build
LIB = -lsodium
INCLUDE_DIR = include
TESTS_DIR = tests
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
EXEC = tests/N00bpass

all: $(EXEC)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $^ -o $@

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LIB)

clean:
	rm -rf $(BUILD_DIR)/*.o $(EXEC) $(TESTS_DIR)/main

