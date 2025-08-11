#ifndef HANDLEINPUT_H
#define HANDLEINPUT_H
#include "cglm/cglm.h"
#include "components.h"
#include <stdbool.h>
#include <stdio.h>

typedef enum { TARGET_NONE, TARGET_CARD, TARGET_UNIT, TARGET_SPELL } TargetType;

typedef struct Hitbox {
  int minX;
  int minY;
  int maxX;
  int maxY;
} Hitbox;

typedef struct Card {
  int manaCost;
  char name[MAXSTRLEN];
  char artPath[MAXSTRLEN];
  int rarity;
  char effectText[MAXSTRLEN];
  int health;
  int attack;
  CardType type;

  int isHovering;
  int isDragging;
  float xpos;
  float ypos;
  float width;
  float height;
  float rotation;

  unsigned int cardVAO;
  unsigned int cardProgram;
} Card;

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
  Hitbox hitbox;
} Unit;

typedef struct Player {
  Card *hand;
  Unit *board;
  Unit *attackingBoard;
  Card *deck;

  int maxMana;
  int currentMana;

  int handSize;
  int unitSize;
} Player;

typedef enum { STATE_NEUTRAL, STATE_TARGETTING } States;

typedef struct GameState {
  Player players[2];
  int turn;
  States state;

} GameState;

typedef struct {

} renderDeck;

ProcessedInput processPlayerInput(GameState *gameState, double xpos,
                                  double ypos, int isClick);

ProcessedInput processPlayerInputECS(ecs_world_t *world);

void ProcessPlayerInputSystem(ecs_iter_t *it);

#endif
