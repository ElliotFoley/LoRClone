#ifndef HANDLEINPUT_H
#define HANDLEINPUT_H
#include "components.h"
#include <stdbool.h>
#include <stdio.h>

typedef enum { TARGET_NONE, TARGET_CARD, TARGET_UNIT, TARGET_SPELL } TargetType;

typedef struct {
  TargetType type;
  int playerId;
  int index;
} InputTarget;

typedef struct {
  InputTarget target;
  int isClick;
  int mouseX, mouseY;
  // bool keyPressed[KEY_COUNT];
} ProcessedInput;

typedef struct {
  char name[MAXSTRLEN];
  char artPath[MAXSTRLEN];
  int rarity;
  char effectText[MAXSTRLEN];
  int health;
  int attack;
} Unit;

/*typedef struct Player {
  Card *hand;
  Unit *board;
  Unit *attackingBoard;
  Card *deck;

  int maxMana;
  int currentMana;

  int handSize;
  int unitSize;
} Player;
*/
typedef enum { STATE_NEUTRAL, STATE_TARGETTING } States;

ProcessedInput processPlayerInputECS(ecs_world_t *world);

void ProcessPlayerInputSystem(ecs_iter_t *it);

#endif
