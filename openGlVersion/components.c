#include "components.h"

ECS_TAG_DECLARE(CardTag);
ECS_TAG_DECLARE(UnitTag);
ECS_TAG_DECLARE(PlayerTag);
//ECS_TAG_DECLARE(BoardTile);
ECS_TAG_DECLARE(IsHovering);
ECS_TAG_DECLARE(IsDragging);

ECS_COMPONENT_DECLARE(ManaCost);
ECS_COMPONENT_DECLARE(Attack);
ECS_COMPONENT_DECLARE(Health);
ECS_COMPONENT_DECLARE(CardType);
ECS_COMPONENT_DECLARE(Owner);
ECS_COMPONENT_DECLARE(Rarity);
ECS_COMPONENT_DECLARE(Name);
ECS_COMPONENT_DECLARE(ArtPath);
ECS_COMPONENT_DECLARE(EffectText);
ECS_COMPONENT_DECLARE(Position);
ECS_COMPONENT_DECLARE(Size);
ECS_COMPONENT_DECLARE(Rotation);
ECS_COMPONENT_DECLARE(VAO);
ECS_COMPONENT_DECLARE(ShaderProgram);

void componentsImport(ecs_world_t *world) {

    ECS_MODULE(world, components);

    ECS_TAG(world, CardTag);
    ECS_TAG(world, UnitTag);
    ECS_TAG(world, PlayerTag);
    ECS_TAG(world, IsHovering);
    ECS_TAG(world, IsDragging);
    //ECS_TAG(world, BoardTile);

    ECS_COMPONENT(world, ManaCost);
    ECS_COMPONENT(world, Attack);
    ECS_COMPONENT(world, Health);
    ECS_COMPONENT(world, CardType);
    ECS_COMPONENT(world, Owner);
    ECS_COMPONENT(world, Rarity);
    ECS_COMPONENT(world, Name);
    ECS_COMPONENT(world, ArtPath);
    ECS_COMPONENT(world, EffectText);
    ECS_COMPONENT(world, Position);
    ECS_COMPONENT(world, Size);
    ECS_COMPONENT(world, Rotation);
    ECS_COMPONENT(world, VAO);
    ECS_COMPONENT(world, ShaderProgram);
}
