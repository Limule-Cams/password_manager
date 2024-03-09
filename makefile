# variable
CC = gcc
CFLAGS = -wall -g 
LIBFLAGS = -lsodium
SRC_DIR = src 
BUILD_DIR = build
BIN_DIR = test



TARGET = $(BIN_DIR)/Noobpass
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

$(TARGET): $(OBJS)
    $(CC) $(CFLAGS) $^ -o $@ $(LIBFLAGS)


$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
    $(CC) $(CFLAGS) -c $< -o $@

clean:
    rm -f $(OBJS) $(TARGET)