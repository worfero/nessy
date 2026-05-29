SRC_DIR = src
OBJ_DIR = build
INCLUDE_DIR = include

CC = g++
CFLAGS = -Wall -I$(INCLUDE_DIR)

SRCS = $(shell find $(SRC_DIR) -type f -name '*.cpp')
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

TARGET = $(OBJ_DIR)/nessy

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ -lSDL2

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(TARGET)

run: $(TARGET)
	@$(TARGET)

nes:
	ca65 asm/main.asm -o asm/main.o
	ld65 -C asm/linker.cfg asm/main.o -o test.nes