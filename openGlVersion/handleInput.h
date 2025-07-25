#IFNDEF HANDLEINPUT_H
#DEFINE HANDLEINPUT_H
#include <stdbool.h>

typedef enum {
    TARGET_NONE,
    TARGET_CARD,
    TARGET_UNIT,
    TARGET_SPELL
} TargetType;

typedef struct {
    TargetType type;
	int playerId;
    int index;
} InputTarget;

typedef struct {
    InputTarget target;
    bool isClick;
    bool isHover;
    int mouseX, mouseY;
    bool keyPressed[KEY_COUNT];
} ProcessedInput;

handleInput


#ENDIF