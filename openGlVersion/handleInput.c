#include "handleInput.h"


int isMouseOverRotatedCard(Card *card, float px, float py) {
    float dx = px - card->xpos;
    float dy = py - card->ypos;

    //float angleRad = glm_rad(-card->rotation); // negate to go from world to local
    //float angleRad = glm_rad(90); // negate to go from world to local
    //float cosA = cosf(angleRad);
    //float sinA = sinf(angleRad);

    // Rotate point into local space of card
    //float localX = dx * cosA - dy * sinA;
    //float localY = dx * sinA + dy * cosA;
    float localX = dx - dy;
    float localY = dx + dy;

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
    Card *player0Hand = gameState->players[PLAYER0].hand;
    Card *player1Hand = gameState->players[PLAYER1].hand;
    ProcessedInput processedInput;
    processedInput.target.type = TARGET_NONE;
    processedInput.isClick = isClick;

    for(int i = 0; i < gameState->players[PLAYER0].handSize; i++){
        if(isMouseOverRotatedCard(&player0Hand[i], xpos, ypos)){
            player0Hand[i].isHovering = 1;
            processedInput.target.playerId = PLAYER0;
            processedInput.target.index = i;
            processedInput.target.type = TARGET_CARD;
        }
        else{
            player0Hand[i].isHovering = 0;
        }
    }
    for(int i = 0; i < gameState->players[PLAYER1].handSize; i++){
        if(isMouseOverRotatedCard(&player1Hand[i], xpos, ypos)){
            player1Hand[i].isHovering = 1;
            processedInput.target.playerId = PLAYER1;
            processedInput.target.index = i;
            processedInput.target.type = TARGET_CARD;
        }
        else{
            player1Hand[i].isHovering = 0;
        }
    }
    return processedInput;
}
