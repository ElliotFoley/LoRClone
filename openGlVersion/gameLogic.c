#include "gameLogic.h"

void removeCard(Player *player, int index){
	for(int i = index; i < player->handSize - 1; i++){
		player->hand[index] = player->hand[index + 1];
	}
	memset(&player->hand[player->handSize - 1], 0, sizeof(Card));
    player->handSize--;
}


void addCard(Player *player, Card cardToAdd){
    if(player->handSize >= MAXHANDSIZE){
        printf("Tried to add card to deck that is full. Card didn't get added\n");
        return;
    }
    player->hand[player->handSize++] = cardToAdd;
}


Card initCard(int manaCost, const char *name, const char *artPath, int rarity, const char *effectText, int health, int attack, CardType type){
    Card card;

    card.manaCost = manaCost;
    card.rarity = rarity;
    card.health = health;
    card.attack = attack;
    card.type = type;
    card.isDragging = 0;

    card.name = malloc(sizeof(char) * MAXSTRLEN);
    sprintf(card.name, "%s", name);
    card.artPath = malloc(sizeof(char) * MAXSTRLEN);
    sprintf(card.artPath, "%s", artPath);
    card.effectText = malloc(sizeof(char) * MAXSTRLEN);
    sprintf(card.effectText, "%s", effectText);


    return card;
}


Player initPlayer(){
    Player player;

    player.hand = malloc(sizeof(Card) * MAXHANDSIZE);
    player.deck = malloc(sizeof(Card) * MAXDECKSIZE);
    player.board = malloc(sizeof(Card) * MAXUNITSIZE);
    player.handSize = 0;
    player.unitSize = 0;

    return player;
}


GameState initGameState(){
    GameState gameState;
    gameState.players[PLAYER0] = initPlayer();
    gameState.players[PLAYER1] = initPlayer();

    Card card = initCard(0, "orca", "", 0, "", 0, 0, 0);
    addCard(&gameState.players[PLAYER0], card);
    addCard(&gameState.players[PLAYER0], card);
    addCard(&gameState.players[PLAYER0], card);
    addCard(&gameState.players[PLAYER0], card);
    addCard(&gameState.players[PLAYER0], card);
    addCard(&gameState.players[PLAYER0], card);

    addCard(&gameState.players[PLAYER1], card);
    addCard(&gameState.players[PLAYER1], card);
    addCard(&gameState.players[PLAYER1], card);
    addCard(&gameState.players[PLAYER1], card);
    addCard(&gameState.players[PLAYER1], card);
    addCard(&gameState.players[PLAYER1], card);

    return gameState;
}


int updateGameState(GameState* gameState, ProcessedInput* userIntent){
	InputTarget input = userIntent->target;
	switch(gameState->state){
		case STATE_NEUTRAL:
			if(input.type == TARGET_NONE)
				break;
			if(input.type == TARGET_CARD){

				//You can't play cards that aren't yours!
				if(input.playerId != gameState->turn)
					break;

				//Card playedCard = gameState->players[gameState->turn].hand[input.index];
				removeCard(&(gameState->players[gameState->turn]), input.index);
				//Send Play card event to event pipeline
				//Add related unit to field

				//Send Summon card event to event pipeline
			}
			break;
	}
    return 0;
}
