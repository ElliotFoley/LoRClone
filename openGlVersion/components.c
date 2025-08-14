#include "components.h"

ECS_TAG_DECLARE(CardTag);
ECS_TAG_DECLARE(UnitTag);
ECS_TAG_DECLARE(PlayerTag);
//ECS_TAG_DECLARE(BoardTile);
ECS_TAG_DECLARE(IsHovering);
ECS_TAG_DECLARE(IsDragging);

ECS_COMPONENT_DECLARE(MousePosition);
ECS_COMPONENT_DECLARE(MouseButtonState);

ECS_COMPONENT_DECLARE(ManaCost);
ECS_COMPONENT_DECLARE(Attack);
ECS_COMPONENT_DECLARE(Health);
ECS_COMPONENT_DECLARE(CardType);
ECS_COMPONENT_DECLARE(Owner);
ECS_COMPONENT_DECLARE(Rarity);
ECS_COMPONENT_DECLARE(Name);
ECS_COMPONENT_DECLARE(ArtPath);
ECS_COMPONENT_DECLARE(EffectText);
ECS_COMPONENT_DECLARE(Index);
ECS_COMPONENT_DECLARE(HandSizes);
ECS_COMPONENT_DECLARE(Position);
ECS_COMPONENT_DECLARE(Size);
ECS_COMPONENT_DECLARE(Rotation);
ECS_COMPONENT_DECLARE(Render);

void componentsImport(ecs_world_t *world) {

    ECS_MODULE(world, components);

    ECS_TAG_DEFINE(world, CardTag);
    ECS_TAG_DEFINE(world, UnitTag);
    ECS_TAG_DEFINE(world, PlayerTag);
    ECS_TAG_DEFINE(world, IsHovering);
    ECS_TAG_DEFINE(world, IsDragging);
    //ECS_TAG(world, BoardTile);

    ECS_COMPONENT_DEFINE(world, MousePosition);
    ECS_COMPONENT_DEFINE(world, MouseButtonState);

    ECS_COMPONENT_DEFINE(world, ManaCost);
    ECS_COMPONENT_DEFINE(world, Attack);
    ECS_COMPONENT_DEFINE(world, Health);
    ECS_COMPONENT_DEFINE(world, CardType);
    ECS_COMPONENT_DEFINE(world, Owner);
    ECS_COMPONENT_DEFINE(world, Rarity);
    ECS_COMPONENT_DEFINE(world, Name);
    ECS_COMPONENT_DEFINE(world, ArtPath);
    ECS_COMPONENT_DEFINE(world, EffectText);
    ECS_COMPONENT_DEFINE(world, Index);
    ECS_COMPONENT_DEFINE(world, HandSizes);
    ECS_COMPONENT_DEFINE(world, Position);
    ECS_COMPONENT_DEFINE(world, Size);
    ECS_COMPONENT_DEFINE(world, Rotation);
    ECS_COMPONENT_DEFINE(world, Render);

    printf("DEBUG: Address in components.c is %p, DEBUG: id = %d\n", (void*)&ecs_id(MousePosition), (int)ecs_id(MousePosition));
    fflush(stdout);
}
