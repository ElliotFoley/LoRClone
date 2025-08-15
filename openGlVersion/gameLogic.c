#include "gameLogic.h"


/*
void addUnitAtIndex(Player *player, Unit unitToAdd, int unitIndex){
    if(player->unitSize >= MAXUNITSIZE){
        printf("Tried to add unit to a board that is full. Card didn't get added\n");
        return;
    }
    for(int i = player->unitSize; i > unitIndex; i--){
       player->board[i] = player->board[i - 1];
    }
    player->board[unitIndex] = unitToAdd;
    player->unitSize++;
}

void playCard(GameState *gameState, int playerId, int cardIndex){
    //Maybe do some checking if playable?
    Player *player = &(gameState->players[playerId]);
    Card playedCard = gameState->players[playerId].hand[cardIndex];
    if(player->currentMana >= playedCard.manaCost){
        //play the card
        removeCard(player, cardIndex);
        player->currentMana -= playedCard.manaCost;
        if(playedCard.type.cardType == CARDTYPE_UNIT){
            //Activate play effect, then summon
            //Here is where I would do a play effect thing
            Unit summonedUnit = cardToUnit(playedCard);
            addUnitAtIndex(player, summonedUnit, player->unitSize);
            //Here is where I would do a summon effect
        }
    }
}
*/


ecs_entity_t initCardECS(ecs_world_t *world, ManaCost manaCost, Name name, ArtPath artPath, Rarity rarity, EffectText effectText, Health health, Attack attack, CardType cardType, Owner owner, Index index, Render render, Zone zone){
    ecs_entity_t card = ecs_new(world);
    //Do note that this copies the data over so these values can go out of scope
    ecs_set_ptr(world, card, ManaCost, &manaCost);
    ecs_set_ptr(world, card, Name, &name);
    ecs_set_ptr(world, card, ArtPath, &artPath);
    ecs_set_ptr(world, card, EffectText, &effectText);
    ecs_set_ptr(world, card, Health, &health);
    ecs_set_ptr(world, card, Attack, &attack);
    ecs_set_ptr(world, card, CardType, &cardType);
    ecs_set_ptr(world, card, Owner, &owner);
    ecs_set_ptr(world, card, Index, &index);
    ecs_set_ptr(world, card, Zone, &zone);

    ecs_set_ptr(world, card, Render, &render);
    ecs_set(world, card, Size, {0, 0});
    ecs_set(world, card, Position, {0, 0});
    ecs_set(world, card, Rotation, {0});
    //Adds the card tag
    ecs_add(world, card, CardTag);

    HandSizes *playerHandSizes = ecs_singleton_get_mut(world, HandSizes);
    playerHandSizes->playerHandSize[owner.playerId]++;

    return card;
}


/*
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
*/
