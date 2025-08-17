#include "handleInput.h"


int isMouseOverRotatedCardECS(Owner owner, Position cardPosition, Size cardSize, Rotation cardRotation, float px, float py) {
    int playerSign = (owner.playerId) ? -1 : 1;
    float centerX = cardPosition.x + cardSize.width / 2.0f * playerSign;
    float centerY = cardPosition.y + cardSize.height / 2.0f * playerSign;

    float dx = px - centerX;
    float dy = py - centerY;

    float angleRad = glm_rad(-cardRotation.angle); // negate to go from world to local
    float cosA = cosf(angleRad);
    float sinA = sinf(angleRad);

    // Rotate point into local space of card
    float localX = dx * cosA - dy * sinA;
    float localY = dx * sinA + dy * cosA;

    float halfW = cardSize.width / 2.0f;
    float halfH = cardSize.height / 2.0f;

    return localX > -halfW && localX < halfW &&
           localY > -halfH && localY < halfH;
}


ProcessedInput processPlayerInputECS(ecs_world_t *world){
    //Mouse input will be with a player entity
    ProcessedInput processedInput;
    processedInput.target.type = TARGET_NONE;
    //processedInput.isClick = isClick;

    ecs_query_desc_t objectQueryDesc = {
        .terms = {
            {.id = ecs_id(Position)},
            {.id = ecs_id(Size)},
            {.id = ecs_id(Rotation)},
            {.id = ecs_id(Owner)},
        }
    };

    ecs_query_t *objectQuery = ecs_query_init(world, &objectQueryDesc);
    ecs_iter_t it = ecs_query_iter(world, objectQuery);

    //outer loop for tables
    int isClicked = 0;
    while(ecs_query_next(&it)){
        Position *p = ecs_field(&it, Position, 0);
        Size *s = ecs_field(&it, Size, 1);
        Rotation *r = ecs_field(&it, Rotation, 2);
        Owner *o = ecs_field(&it, Owner, 3);

        //inner loop for enities in the table
        for(int i = 0; i < it.count; i++){
            ecs_entity_t e = it.entities[i];
            if(isMouseOverRotatedCardECS(o[i], p[i], s[i], r[i], 0, 0) && !isClicked){
                ecs_add(world, e, IsHovering);
                /*if(isclick){
                    ecs_add(world, e, IsDragging);
                }
                */
                isClicked = 1;
            }
            else{
                ecs_remove(world, e, IsHovering);
                ecs_remove(world, e, IsDragging);
            }
        }
    }

    return processedInput;

}

void ProcessPlayerInputSystem(ecs_iter_t *it){
    Position *p = ecs_field(it, Position, 0);
    Size *s = ecs_field(it, Size, 1);
    Rotation *r = ecs_field(it, Rotation, 2);
    Owner *o = ecs_field(it, Owner, 3);

    const MousePosition *mousePos = ecs_singleton_get(it->world, MousePosition);
    const MouseButtonState *mouseState = ecs_singleton_get(it->world, MouseButtonState);
    EntitySelectedState *selectedState = ecs_singleton_get_mut(it->world, EntitySelectedState);
    if(!mouseState->leftDown){
        selectedState->state = 0;
    }
    //This starts at 10000 because it->count will probably not be that big if it->count > 10000 this will break
    int isDraggingIndex = 10000;
    if(mouseState->leftDown){
        for(int i = 0; i < it->count; i++){
            if(ecs_has(it->world, it->entities[i], IsDragging)){
                isDraggingIndex = i;
            }
        }
    }
    //inner loop for enities in the table
    for(int i = 0; i < it->count; i++){
        //This needs to be skipped so I don't remove IsHovering and IsDragging from the selected entity
        if(i == isDraggingIndex){
            continue;
        }
        ecs_entity_t e = it->entities[i];
        if(isMouseOverRotatedCardECS(o[i], p[i], s[i], r[i], mousePos->x, mousePos->y) && !selectedState->state){
            ecs_add(it->world, e, IsHovering);
            if(mouseState->leftDown){
                ecs_add(it->world, e, IsDragging);
                selectedState->state = 1;
            }
            else{
                ecs_remove(it->world, e, IsDragging);
            }
        }
        else{
            ecs_remove(it->world, e, IsHovering);
            ecs_remove(it->world, e, IsDragging);
        }
    }
}
