#IFNDEF GAMELOGIC_H
#DEFINE GAMELOGIC_H

#include "handleInput.h"

typedef struct Hitbox {
	int minX;
	int minY;
	int maxX;
	int maxY;
} Hitbox;

typedef enum {
	CARDTYPE_UNIT,
	CARDTYPE_SPELL
} CardType;

typedef struct Card {
	int manaCost;
	char* name;
	char* artPath;
	int rarity;
	char* effectText;
	int health;
	int attack;
	CardType type;
	Hitbox hitbox;
} Card;

void eventListner(Card *card, Card *eventCard)

typedef struct Unit{
	char* name;
	char* artPath;
	int rarity;
	char* effectText;
	int health;
	int attack;
	Hitbox hitbox;
} Unit;

typedef struct Player {
	Card* hand;
	Unit* board;
	Unit* attackingBoard;
	Card* deck;
} Player;
void playCard(Card card);
void declareAttack(Unit* unitArray);

typedef enum {
	STATE_NEUTRAL,
	STATE_TARGETTING
} States;

typedef struct GameState {
	Player players[2];
	int turn;
	States state;
	
} GameState;
void resolveAttack(GameState gameState);

#ENDIF