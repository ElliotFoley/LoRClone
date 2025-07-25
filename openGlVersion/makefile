# Detect platform
ifeq ($(OS),Windows_NT)
    # Windows (MSYS2 / MinGW)
    LIBS = -lglfw3 -lgdi32 -lopengl32 -lm
else
    # Assume Linux
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        LIBS = -lglfw -lGL -lm
    endif
endif

# Compiler and flags
CC = gcc
CFLAGS = -Wall
SOURCES = main.c glad.c
TARGET = main

# Build rule
all: $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES) $(LIBS)
