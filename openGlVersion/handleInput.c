#include "handleInput.h"


int isMouseOverRotatedCard(int playerId, Card *card, float px, float py) {
    int playerSign = (playerId) ? -1 : 1;
    float centerX = card->xpos + card->width / 2.0f * playerSign;
    float centerY = card->ypos + card->height / 2.0f * playerSign;

    float dx = px - centerX;
    float dy = py - centerY;

    float angleRad = glm_rad(-card->rotation); // negate to go from world to local
    float cosA = cosf(angleRad);
    float sinA = sinf(angleRad);

    // Rotate point into local space of card
    float localX = dx * cosA - dy * sinA;
    float localY = dx * sinA + dy * cosA;

    float halfW = card->width / 2.0f;
    float halfH = card->height / 2.0f;

    return localX > -halfW && localX < halfW &&
           localY > -halfH && localY < halfH;
}


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
    ProcessedInput processedInput;
    processedInput.target.type = TARGET_NONE;
    processedInput.isClick = isClick;

    for(int player = 0; player < 2; player++){
        Card *playerHand = gameState->players[player].hand;
        for(int i = 0; i < gameState->players[player].handSize; i++){
            if(isMouseOverRotatedCard(player, &playerHand[i], xpos, ypos)){
                if(isClick){
                    playerHand[i].isDragging = 1;
                }
                else{
                    playerHand[i].isDragging = 0;
                }
                playerHand[i].isHovering = 1;
                processedInput.target.playerId = PLAYER0;
                processedInput.target.index = i;
                processedInput.target.type = TARGET_CARD;
            }
            else{
                playerHand[i].isDragging = 0;
                playerHand[i].isHovering = 0;
            }

        }
    }

    return processedInput;
}
