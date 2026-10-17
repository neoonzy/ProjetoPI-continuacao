# Build with a system raylib (Linux/macOS) or a raylib installation (Windows).
CC = gcc
TARGET ?= cintype
SOURCES := main.c $(wildcard screens/*.c) $(wildcard entities/*.c) $(filter-out mecanics/power_up.c,$(wildcard mecanics/*.c))
OBJECTS := $(SOURCES:.c=.o)
CFLAGS += -std=c99 -O2 -Wall -Wextra -Wpedantic -I.

ifeq ($(OS),Windows_NT)
    EXT := .exe
    RAYLIB_PATH ?= C:/raylib/raylib
    CFLAGS += -I$(RAYLIB_PATH)/src
    LDLIBS += -L$(RAYLIB_PATH)/src -lraylib -lopengl32 -lgdi32 -lwinmm
else
    CFLAGS += $(shell pkg-config --cflags raylib 2>/dev/null)
    LDLIBS += $(shell pkg-config --libs raylib 2>/dev/null) -lm
endif
ifeq ($(BUILD_MODE),DEBUG)
    CFLAGS += -g -O0
endif

.PHONY: all clean
all: $(TARGET)$(EXT)

$(TARGET)$(EXT): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDLIBS)

%.o: %.c game.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)$(EXT)
