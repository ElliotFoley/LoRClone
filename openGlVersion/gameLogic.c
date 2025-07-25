#include "gameLogic.h"

void removeCard(Card *hand, int handSize, int index){
	for(int i = index; i < handSize - 1; i++){
		hand[index] = hand[index + 1];
	}
	hand[handSize - 1] = {0};
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
				
				Card playedCard = gameState->players[gameState->turn].hand[input.index];
				removeCard(&(gameState->players[gameState->turn].hand), MAXHANDSIZE, input.index);
				//Send Play card event to event pipeline
				//Add related unit to field
				
				//Send Summon card event to event pipeline
			}
			break;
	}
}

struct