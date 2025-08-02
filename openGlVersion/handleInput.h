#ifndef HANDLEINPUT_H
#define HANDLEINPUT_H
#include "define.h"
#include <stdbool.h>

typedef enum { TARGET_NONE, TARGET_CARD, TARGET_UNIT, TARGET_SPELL } TargetType;

typedef struct Hitbox {
  int minX;
  int minY;
  int maxX;
  int maxY;
} Hitbox;

typedef enum { CARDTYPE_UNIT, CARDTYPE_SPELL } CardType;

typedef struct Card {
  int manaCost;
  char *name;
  char *artPath;
  int rarity;
  char *effectText;
  int health;
  int attack;
  CardType type;
  Hitbox hitbox;

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
  char *name;
  char *artPath;
  int rarity;
  char *effectText;
  int health;
  int attack;
  Hitbox hitbox;
} Unit;

typedef struct Player {
  Card *hand;
  Unit *board;
  Unit *attackingBoard;
  Card *deck;
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

// handleInput

#endif
