#IFNDEF GAMELOGIC_H
#DEFINE GAMELOGIC_H

typedef struct Card {
	int manaCost;
	char* name;
	char* artPath;
	int rarity;
	char* effectText;
	int health;
	int attack;
} Card;

typedef struct Unit{
	char* name;
	char* artPath;
	int rarity;
	char* effectText;
	int health;
	int attack;
} Unit;

typedef struct Player {
	Card* hand;
	Unit* board;
	Unit* attackingBoard;
	Card* deck;
} Player;
playCard(Card card);
declareAttack(Unit* unitArray);

typedef struct GameState {
	Player player1;
	Player player2;
} GameState;
void resolveAttack(GameState gameState);

#ENDIF