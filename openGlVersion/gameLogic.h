#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include "handleInput.h"
#include <string.h>

void eventListner(Card *card, Card *eventCard);

void playCard(Card card);
void declareAttack(Unit *unitArray);

void resolveAttack(GameState gameState);

#endif
