# -------------------------------------------------
# Platform detection
# -------------------------------------------------
ifeq ($(OS),Windows_NT)
    # Windows (MSYS2 / MinGW)
    LIBS = -lglfw3 -lgdi32 -lopengl32 -lm -lws2_32
    FREETYPE_CFLAGS = -IC:/msys64/mingw64/include/freetype2
    FREETYPE_LIBS   = -LC:/msys64/mingw64/lib -lfreetype
else
    # Assume Linux / Unix
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        FREETYPE_CFLAGS := $(shell pkg-config --cflags freetype2)
        FREETYPE_LIBS   := $(shell pkg-config --libs freetype2)
        LIBS = -lglfw -lGL -lm
    endif
endif



# -------------------------------------------------
# Compiler and flags
# -------------------------------------------------
CC      = gcc
CFLAGS  = -Wall $(FREETYPE_CFLAGS)
LDFLAGS = $(LIBS) $(FREETYPE_LIBS)

# -------------------------------------------------
# Sources / target
# -------------------------------------------------
SOURCES = main.c glad.c flecs/flecs.c components.c gameLogic.c handleInput.c
TARGET  = main

# -------------------------------------------------
# Build rules
# -------------------------------------------------
all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(TARGET) *.o
