#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include "handleInput.h"
#include <stdlib.h>
#include <string.h>

void eventListner(Card *card, Card *eventCard);

void removeCard(Player *player, int index);
void playCard(GameState *gameState, int playerId, int cardIndex);
void declareAttack(Unit *unitArray);
void addCard(Player *player, Card cardToAdd);
ecs_entity_t initCardECS(ecs_world_t *world, ManaCost manaCost, Name name,
                         ArtPath artPath, Rarity rarity, EffectText effectText,
                         Health health, Attack attack, CardType cardType,
                         Owner owner, Index index, Render render);

GameState initGameState();
Player initPlayer();

void resolveAttack(GameState gameState);

#endif
