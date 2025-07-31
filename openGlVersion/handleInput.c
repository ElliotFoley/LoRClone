#include "handleInput.h"

ProcessedInput processInput(GameState *gameState, double xpos, double ypos, int isClick){
    Card *player0Deck = gameState->players[PLAYER0].deck;
    Card *player1Deck = gameState->players[PLAYER1].deck;
    ProcessedInput processedInput;
    processedInput.isClick = isClick;

    for(int i = 0; i < gameState->players[PLAYER0].handSize; i++){
        if(xpos > player0Deck[i].hitbox.minX && xpos < player0Deck[i].hitbox.maxX && ypos > player0Deck[i].hitbox.minY && ypos < player0Deck[i].hitbox.maxY){
            processedInput.target.playerId = PLAYER0;
            processedInput.target.index = i;
            processedInput.target.type = TARGET_CARD;
            return processedInput;
        }
    }
    for(int i = 0; i < gameState->players[PLAYER1].handSize; i++){
        if(xpos > player1Deck[i].hitbox.minX && xpos < player1Deck[i].hitbox.maxX && ypos > player1Deck[i].hitbox.minY && ypos < player1Deck[i].hitbox.maxY){
            processedInput.target.playerId = PLAYER1;
            processedInput.target.index = i;
            processedInput.target.type = TARGET_CARD;
            return processedInput;
        }
    }
    processedInput.target.type = TARGET_NONE;
    return processedInput;
}
