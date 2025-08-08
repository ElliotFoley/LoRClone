#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include "handleInput.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void eventListner(Card *card, Card *eventCard);

void removeCard(Player *player, int index);
void playCard(GameState *gameState, int playerId, int cardIndex);
void declareAttack(Unit *unitArray);
void addCard(Player *player, Card cardToAdd);

GameState initGameState();
Player initPlayer();

void resolveAttack(GameState gameState);

#endif
