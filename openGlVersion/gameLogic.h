#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include "handleInput.h"
#include <stdlib.h>
#include <string.h>

// void eventListner(Card *card, Card *eventCard);

// void declareAttack(Unit *unitArray);
ecs_entity_t initCardECS(ecs_world_t *world, ManaCost manaCost, Name name,
                         ArtPath artPath, Rarity rarity, EffectText effectText,
                         Health health, Attack attack, CardType cardType,
                         Owner owner, Index index, Render render);

// void resolveAttack(GameState gameState);

#endif
