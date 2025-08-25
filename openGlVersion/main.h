#ifndef MAIN_H
#define MAIN_H

#include <ctype.h>

#include "gameLogic.h"

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <ft2build.h>
#include FT_FREETYPE_H

typedef struct {
  ecs_entity_t mouseEntity;
} DataWrapper;

enum TextAnchor { ANCHOR_BOTTOM_LEFT, ANCHOR_TOP_LEFT, ANCHOR_BOTTOM_RIGHT };

void renderText(unsigned int VAO, unsigned int VBO, unsigned int shaderProgram,
                Character *glyphMap, char *text, float x, float y, float scale,
                float rotation, vec3 textColor, float localXOffset,
                float localYOffset);

#endif
