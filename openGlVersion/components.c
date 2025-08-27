#include "components.h"

//Unit types
ECS_TAG_DECLARE(Robot);
ECS_TAG_DECLARE(Penguin);

ECS_TAG_DECLARE(CardTag);
ECS_TAG_DECLARE(UnitTag);
ECS_TAG_DECLARE(PlayerTag);
//ECS_TAG_DECLARE(BoardTile);
ECS_TAG_DECLARE(IsHovering);
ECS_TAG_DECLARE(IsDragging);

ECS_COMPONENT_DECLARE(RenderText);
ECS_COMPONENT_DECLARE(MousePosition);
ECS_COMPONENT_DECLARE(MouseButtonState);
ECS_COMPONENT_DECLARE(EntitySelectedState);

ECS_COMPONENT_DECLARE(Zone);
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
ECS_COMPONENT_DECLARE(BoardSizes);
ECS_COMPONENT_DECLARE(DeckSizes);
ECS_COMPONENT_DECLARE(Position);
ECS_COMPONENT_DECLARE(Size);
ECS_COMPONENT_DECLARE(Rotation);
ECS_COMPONENT_DECLARE(Render);

ECS_COMPONENT_DECLARE(OnPlay);
ECS_COMPONENT_DECLARE(OnSummon);
ECS_COMPONENT_DECLARE(OnDeath);
ECS_COMPONENT_DECLARE(OnAura);
ECS_COMPONENT_DECLARE(OnStartTurn);
ECS_COMPONENT_DECLARE(OnEndTurn);

void componentsImport(ecs_world_t *world) {

    ECS_MODULE(world, components);

    ECS_TAG_DEFINE(world, Robot);
    ECS_TAG_DEFINE(world, Penguin);

    ECS_TAG_DEFINE(world, CardTag);
    ECS_TAG_DEFINE(world, UnitTag);
    ECS_TAG_DEFINE(world, PlayerTag);
    ECS_TAG_DEFINE(world, IsHovering);
    ECS_TAG_DEFINE(world, IsDragging);
    //ECS_TAG(world, BoardTile);

    ECS_COMPONENT_DEFINE(world, RenderText);
    ECS_COMPONENT_DEFINE(world, MousePosition);
    ECS_COMPONENT_DEFINE(world, MouseButtonState);
    ECS_COMPONENT_DEFINE(world, EntitySelectedState);

    ECS_COMPONENT_DEFINE(world, Zone);
    ecs_entity_t zoneEnumId = ecs_enum_init(world, &(ecs_enum_desc_t) {
        .entity = ecs_entity(world, { .name = "ZoneEnum" }),
        .constants = {
            { .name = "ZONE_DECK" },
            { .name = "ZONE_HAND" },
            { .name = "ZONE_BOARD" },
            { .name = "ZONE_ATTACK_BOARD" },
            { .name = "ZONE_DISCARD" }
        }
    });

    ecs_struct(world, {
        .entity = ecs_id(Zone),
        .members = {
            { .name = "zone", .type = zoneEnumId }
        }
    });

    ECS_COMPONENT_DEFINE(world, ManaCost);
    ecs_struct(world, {
        .entity = ecs_id(ManaCost),
        .members = {
            { .name = "manaCost", .type = ecs_id(ecs_i32_t) }
        }
    });

    ECS_COMPONENT_DEFINE(world, Attack);
    ecs_struct(world, {
        .entity = ecs_id(Attack),
        .members = {
            { .name = "attack", .type = ecs_id(ecs_i32_t) }
        }
    });

    ECS_COMPONENT_DEFINE(world, Health);
    ecs_struct(world, {
        .entity = ecs_id(Health),
        .members = {
            { .name = "manaCost", .type = ecs_id(ecs_i32_t) }
        }
    });

    ECS_COMPONENT_DEFINE(world, CardType);
    ecs_entity_t cardTypeEnumId = ecs_enum_init(world, &(ecs_enum_desc_t) {
        .entity = ecs_entity(world, { .name = "CardTypeEnum" }),
        .constants = {
            { .name = "CARDTYPE_UNIT" },
            { .name = "CARDTYPE_SPELL" }
        }
    });

    ecs_struct(world, {
        .entity = ecs_id(CardType),
        .members = {
            { .name = "type", .type = cardTypeEnumId }
        }
    });

    ECS_COMPONENT_DEFINE(world, Owner);
    ecs_struct(world, {
        .entity = ecs_id(Owner),
        .members = {
            { .name = "playerId", .type = ecs_id(ecs_i32_t) }
        }
    });

    ECS_COMPONENT_DEFINE(world, Rarity);
    ecs_struct(world, {
        .entity = ecs_id(Rarity),
        .members = {
            { .name = "rarity", .type = ecs_id(ecs_i32_t) }
        }
    });

    ECS_COMPONENT_DEFINE(world, Name);
    ecs_struct(world, {
        .entity = ecs_id(Name),
        .members = {
            { .name = "name", .type = ecs_id(ecs_string_t) }
        }
    });

    ECS_COMPONENT_DEFINE(world, ArtPath);
    ecs_struct(world, {
        .entity = ecs_id(ArtPath),
        .members = {
            { .name = "artPath", .type = ecs_id(ecs_string_t) }
        }
    });

    ECS_COMPONENT_DEFINE(world, EffectText);
    ecs_struct(world, {
        .entity = ecs_id(EffectText),
        .members = {
            { .name = "effectText", .type = ecs_id(ecs_string_t) }
        }
    });

    ECS_COMPONENT_DEFINE(world, Index);
    ecs_struct(world, {
        .entity = ecs_id(Index),
        .members = {
            { .name = "index", .type = ecs_id(ecs_i32_t) }
        }
    });

    ECS_COMPONENT_DEFINE(world, HandSizes);
    ecs_struct(world, {
        .entity = ecs_id(HandSizes),
        .members = {
            { .name = "playerHandSize", .type = ecs_id(ecs_i32_t), .count = 2 }
        }
    });

    ECS_COMPONENT_DEFINE(world, BoardSizes);
    ecs_struct(world, {
        .entity = ecs_id(BoardSizes),
        .members = {
            { .name = "playerUnitSize", .type = ecs_id(ecs_i32_t), .count = 2 }
        }
    });

    ECS_COMPONENT_DEFINE(world, DeckSizes);
    ecs_struct(world, {
        .entity = ecs_id(DeckSizes),
        .members = {
            { .name = "playerDeckSize", .type = ecs_id(ecs_i32_t), .count = 2 }
        }
    });

    ECS_COMPONENT_DEFINE(world, Position);
    ecs_struct(world, {
        .entity = ecs_id(Position),
        .members = {
            { .name = "x", .type = ecs_id(ecs_f32_t) },
            { .name = "y", .type = ecs_id(ecs_f32_t) }
        }
    });

    ECS_COMPONENT_DEFINE(world, Size);
    ecs_struct(world, {
        .entity = ecs_id(Size),
        .members = {
            { .name = "width", .type = ecs_id(ecs_f32_t) },
            { .name = "height", .type = ecs_id(ecs_f32_t) }
        }
    });

    ECS_COMPONENT_DEFINE(world, Rotation);
    ecs_struct(world, {
        .entity = ecs_id(Rotation),
        .members = {
            { .name = "angle", .type = ecs_id(ecs_f32_t) }
        }
    });

    ECS_COMPONENT_DEFINE(world, Render);
    ecs_struct(world, {
        .entity = ecs_id(Render),
        .members = {
            { .name = "shaderProgram", .type = ecs_id(ecs_i32_t) },
            { .name = "vao", .type = ecs_id(ecs_i32_t) },
            { .name = "texture", .type = ecs_id(ecs_i32_t) }
        }
    });

    ECS_COMPONENT_DEFINE(world, OnPlay);
    ecs_entity_t effectTypeEnumId = ecs_enum_init(world, &(ecs_enum_desc_t) {
        .entity = ecs_entity(world, { .name = "EffectType" }),
        .constants = {
            { .name = "EFFECT_DRAW" },
            { .name = "EFFECT_BUFF_ALLIES" },
            { .name = "EFFECT_BUFF_SELF" },
            { .name = "EFFECT_SCRY" },
            { .name = "EFFECT_PENGUIN_KING" },
            { .name = "EFFECT_DAMAGE_ENEMY_NEXUS" },
            { .name = "EFFECT_FRIENDLY_DUCK" },
            { .name = "EFFECT_GIVE_SELF_KEYWORD" },
            { .name = "EFFECT_GIVE_SELF_RANDOM_KEYWORDS" },
            { .name = "EFFECT_SILENT_ASSASSIN" },
            { .name = "EFFECT_SILENCE_SELF" },
            { .name = "EFFECT_ADJUST_MANA" },
            { .name = "EFFECT_SET_MANA" },
            { .name = "EFFECT_CLEAR_SKYS_SET_MANA" },
            { .name = "EFFECT_PENGUIN_PALS" },
            { .name = "EFFECT_AUDIO_AMPLIFIER" },
            { .name = "EFFECT_DIE" },
            { .name = "EFFECT_SELF_HEAL" },
            { .name = "EFFECT_HEAL_OWN_NEXUS" },
        }
    });

    ecs_struct(world, {
        .entity = ecs_id(OnPlay),
        .members = {
            { .name = "type", .type = effectTypeEnumId }
        }
    });

    ECS_COMPONENT_DEFINE(world, OnSummon);
    ecs_struct(world, {
        .entity = ecs_id(OnSummon),
        .members = {
            { .name = "type", .type = effectTypeEnumId }
        }
    });

    ECS_COMPONENT_DEFINE(world, OnDeath);
    ecs_struct(world, {
        .entity = ecs_id(OnDeath),
        .members = {
            { .name = "type", .type = effectTypeEnumId }
        }
    });

    ECS_COMPONENT_DEFINE(world, OnAura);
    ecs_struct(world, {
        .entity = ecs_id(OnAura),
        .members = {
            { .name = "type", .type = effectTypeEnumId }
        }
    });

    ECS_COMPONENT_DEFINE(world, OnStartTurn);
    ecs_struct(world, {
        .entity = ecs_id(OnStartTurn),
        .members = {
            { .name = "type", .type = effectTypeEnumId }
        }
    });

    ECS_COMPONENT_DEFINE(world, OnEndTurn);
    ecs_struct(world, {
        .entity = ecs_id(OnEndTurn),
        .members = {
            { .name = "type", .type = effectTypeEnumId }
        }
    });

}
