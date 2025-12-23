CC = cc
BUILD_DIR = build
SOURCES_DIR = src
HEADER_DIR = include

SOURCES = $(SOURCES_DIR)/main.c \
          $(SOURCES_DIR)/queue.c \
          $(SOURCES_DIR)/sort.c \
          $(SOURCES_DIR)/file_io.c \
          $(SOURCES_DIR)/benchmark.c

OBJECTS = $(SOURCES:$(SOURCES_DIR)/%.c=$(BUILD_DIR)/%.o)

TARGET = $(BUILD_DIR)/prog.out

all: $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(TARGET): $(OBJECTS)
	$(CC) -o $@ $^ -lm

$(BUILD_DIR)/%.o: $(SOURCES_DIR)/%.c | $(BUILD_DIR)
	$(CC) -c $< -o $@ -I$(HEADER_DIR) -Iinclude -Wall -Wextra -O2

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: all clean