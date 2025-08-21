#ifndef MAIN_H
#define MAIN_H

#include "gameLogic.h"

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <ft2build.h>
#include FT_FREETYPE_H

typedef struct {
  ecs_entity_t mouseEntity;
} DataWrapper;

#endif
