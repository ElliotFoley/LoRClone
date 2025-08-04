#include "handleInput.h"


Hitbox getCardHitbox(Card *card){
    float halfW = card->width / 2.0f;
    float halfH = card->height / 2.0f;

    return (Hitbox){
        .minX = card->xpos - halfW,
        .maxX = card->xpos + halfW,
        .minY = card->ypos - halfH,
        .maxY = card->ypos + halfH
    };
}


ProcessedInput processPlayerInput(GameState *gameState, double xpos, double ypos, int isClick){
    Card *player0Deck = gameState->players[PLAYER0].deck;
    Card *player1Deck = gameState->players[PLAYER1].deck;
    ProcessedInput processedInput;
    processedInput.isClick = isClick;

    for(int i = 0; i < gameState->players[PLAYER0].handSize; i++){
        Hitbox hitbox = getCardHitbox(&player0Deck[i]);
        if(xpos > hitbox.minX && xpos < hitbox.maxX && ypos > hitbox.minY && ypos < hitbox.maxY){
            processedInput.target.playerId = PLAYER0;
            processedInput.target.index = i;
            processedInput.target.type = TARGET_CARD;
            return processedInput;
        }
    }
    for(int i = 0; i < gameState->players[PLAYER1].handSize; i++){
        Hitbox hitbox = getCardHitbox(&player1Deck[i]);
        if(xpos > hitbox.minX && xpos < hitbox.maxX && ypos > hitbox.minY && ypos < hitbox.maxY){
            processedInput.target.playerId = PLAYER1;
            processedInput.target.index = i;
            processedInput.target.type = TARGET_CARD;
            return processedInput;
        }
    }
    processedInput.target.type = TARGET_NONE;
    return processedInput;
}
