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


void playCardECS(ecs_iter_t *it, Owner owner, ecs_entity_t cardToPlay){
    Index *cardToPlayIndex = ecs_get_mut(it->world, cardToPlay, Index);
    Zone *cardToPlayZone = ecs_get_mut(it->world, cardToPlay, Zone);
    cardToPlayZone->zone = ZONE_BOARD;
    BoardSizes *playerBoardSizes = ecs_singleton_get_mut(it->world, BoardSizes);
    HandSizes *playerHandSizes = ecs_singleton_get_mut(it->world, HandSizes);
    int oldIndex = cardToPlayIndex->index;
    cardToPlayIndex->index = playerBoardSizes->playerBoardSize[owner.playerId]++;
    playerHandSizes->playerHandSize[owner.playerId]--;
    ecs_remove(it->world, cardToPlay, CardType);
    ecs_remove(it->world, cardToPlay, CardTag);
    ecs_add(it->world, cardToPlay, UnitTag);

    for(int i = 0; i < it->count; i++){
        const Owner *eOwner = ecs_get(it->world, it->entities[i], Owner);
		const Zone *zone = ecs_get(it->world, it->entities[i], Zone);
        if(zone->zone == ZONE_HAND && eOwner->playerId == owner.playerId){
            Index *index = ecs_get_mut(it->world, it->entities[i], Index);
            if(index->index > oldIndex){
                index->index--;
            }
        }
    }

}

void drawCardECS(ecs_iter_t *it, Owner owner){


	DeckSizes *playerDeckSizes = ecs_singleton_get_mut(it->world, DeckSizes);
	int cardToDrawI = -1;
	for(int i = 0; i < it->count; i++){
        const Owner *eOwner = ecs_get(it->world, it->entities[i], Owner);
        Index *index = ecs_get_mut(it->world, it->entities[i], Index);
		const Zone *zone = ecs_get(it->world, it->entities[i], Zone);
        if(zone->zone == ZONE_DECK && eOwner->playerId == owner.playerId && index->index == playerDeckSizes->playerDeckSize[owner.playerId] - 1){
            cardToDrawI = i;
            break;
        }
    }
    if(cardToDrawI == -1){
        return;
    }


    Zone *cardToPlayZone = ecs_get_mut(it->world, it->entities[cardToDrawI], Zone);
    cardToPlayZone->zone = ZONE_HAND;
    HandSizes *playerHandSizes = ecs_singleton_get_mut(it->world, HandSizes);

    playerHandSizes->playerHandSize[owner.playerId]++;
    playerDeckSizes->playerDeckSize[owner.playerId]--;
    Index *cardToDrawIndex = ecs_get_mut(it->world, it->entities[cardToDrawI], Index);
	cardToDrawIndex->index = playerHandSizes->playerHandSize[owner.playerId] - 1;


}


ecs_entity_t getCardFromIndex(ecs_iter_t *it, Owner owner, int index){
    for(int i = 0; i < it->count; i++){
        const Owner *cardOwner = ecs_get(it->world, it->entities[i], Owner);
        const Index *cardIndex = ecs_get(it->world, it->entities[i], Index);
        const Zone *zone = ecs_get(it->world, it->entities[i], Zone);
        if(cardIndex->index == index && cardOwner->playerId == owner.playerId && zone->zone == ZONE_HAND){
            return it->entities[i];
        }
    }
    return -1;
}


void gameStateSystem(ecs_iter_t *it){
    Health *health = ecs_field(it, Health, 0);
    ManaCost *manaCost = ecs_field(it, ManaCost, 1);
    Attack *attack = ecs_field(it, Attack, 2);
    InputState *inputState = ecs_singleton_get_mut(it->world, InputState);

    //NEED TO MAKE THIS A PART OF A GAMESTATE COMPONENT
    Owner playersTurn;
    playersTurn.playerId = PLAYER0;

    for(int i = 0; i < 10; i++){
        if(inputState->numKeys[i]){
            ecs_entity_t cardToPlayId = getCardFromIndex(it, playersTurn, i);
            if(cardToPlayId != -1){
                playCardECS(it, playersTurn, cardToPlayId);
                inputState->numKeys[i] = 0;
            }
        }
    }
}


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


ecs_entity_t initUnitECS(ecs_world_t *world, ManaCost manaCost, Name name, ArtPath artPath, Rarity rarity, EffectText effectText, Health health, Attack attack, Owner owner, Index index, Render render, Zone zone){
    ecs_entity_t unit = ecs_new(world);
    //Do note that this copies the data over so these values can go out of scope
    ecs_set_ptr(world, unit, ManaCost, &manaCost);
    ecs_set_ptr(world, unit, Name, &name);
    ecs_set_ptr(world, unit, ArtPath, &artPath);
    ecs_set_ptr(world, unit, EffectText, &effectText);
    ecs_set_ptr(world, unit, Health, &health);
    ecs_set_ptr(world, unit, Attack, &attack);
    ecs_set_ptr(world, unit, Owner, &owner);
    ecs_set_ptr(world, unit, Index, &index);
    ecs_set_ptr(world, unit, Zone, &zone);

    ecs_set_ptr(world, unit, Render, &render);
    ecs_set(world, unit, Size, {0, 0});
    ecs_set(world, unit, Position, {0, 0});
    ecs_set(world, unit, Rotation, {0});
    //Adds the card tag
    ecs_add(world, unit, UnitTag);

    BoardSizes *playerBoardSizes = ecs_singleton_get_mut(world, BoardSizes);
    playerBoardSizes->playerBoardSize[owner.playerId]++;

    return unit;
}

char *readFileToString(const char *path) {
    FILE *f = fopen(path, "rb");
    if (!f) {
        fprintf(stderr, "Could not open file: %s\n", path);
        return NULL;
    }

    fseek(f, 0, SEEK_END);
    long len = ftell(f);
    rewind(f);

    char *buf = malloc(len + 1);
    if (!buf) {
        fclose(f);
        return NULL;
    }

    fread(buf, 1, len, f);
    buf[len] = '\0';
    fclose(f);

    return buf;
}

void decklistToDeckECS(ecs_world_t *world, const char *decklist_path) {
	FILE *decklist = fopen(decklist_path, "r");
    if (!decklist) {
        fprintf(stderr, "Could not open decklist: %s\n", decklist_path);
        return;
    }

    char line[512];
    while (fgets(line, sizeof(line), decklist)) {
        // Remove trailing newline
        line[strcspn(line, "\r\n")] = 0;

        // Read JSON file for this card
        char *json = readFileToString(line);
        if (!json) continue;

        // Create ECS entity and load JSON into it
        ecs_entity_t card = ecs_new(world);
        const char *res = ecs_entity_from_json(world, card, json, NULL);
		if (!res) {
			fprintf(stderr, "Failed to parse JSON for entity %llu\n", (unsigned long long)card);
		}

        free(json);
    }

    fclose(decklist);
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
