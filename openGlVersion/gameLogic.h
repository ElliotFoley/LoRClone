#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include "handleInput.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void eventListner(Card *card, Card *eventCard);

void removeCard(Player *player, int index);
void playCard(Card card);
void declareAttack(Unit *unitArray);
void addCard(Player *player, Card cardToAdd);

GameState initGameState();
Player initPlayer();

void resolveAttack(GameState gameState);

#endif
