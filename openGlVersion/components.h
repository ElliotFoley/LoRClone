#ifndef COMPONENTS_H
#define COMPONENTS_H
#include "define.h"
#include "flecs/flecs.h"
#include <stdio.h>

// components for cards for ECS
typedef struct {
  enum {
    ZONE_DECK,
    ZONE_HAND,
    ZONE_BOARD,
    ZONE_ATTACK_BOARD,
    ZONE_DISCARD
  } zone;
} Zone;
typedef struct {
  int manaCost;
} ManaCost;
typedef struct {
  int attack;
} Attack;
typedef struct {
  int health;
} Health;
typedef struct {
  enum { CARDTYPE_UNIT, CARDTYPE_SPELL } cardType;
} CardType;
typedef struct {
  int playerId;
} Owner;
typedef struct {
  int rarity;
} Rarity;

typedef struct {
  char name[MAXSTRLEN];
} Name;
typedef struct {
  char artPath[MAXSTRLEN];
} ArtPath;
typedef struct {
  char effectText[MAXSTRLEN];
} EffectText;

typedef struct {
  // 2 total players
  int playerHandSize[2];
} HandSizes;
typedef struct {
  int index;
} Index;
typedef struct {
  float x, y;
} Position;
typedef struct {
  float width, height;
} Size;
typedef struct {
  float angle;
} Rotation;
typedef struct {
  unsigned int shaderProgram;
  unsigned int vao;
  unsigned int texture;
} Render;

typedef struct {
  float x, y;
} MousePosition;

typedef struct {
  int leftDown;
} MouseButtonState;

typedef enum {
  EFFECT_DRAW,
  EFFECT_BUFF_ALLIES,
  EFFECT_BUFF_SELF,
  EFFECT_SCRY,
  EFFECT_PENGUIN_KING,
  EFFECT_DAMAGE_ENEMY_NEXUS,
  EFFECT_FRIENDLY_DUCK,
  EFFECT_GIVE_SELF_KEYWORD,
  EFFECT_GIVE_SELF_RANDOM_KEYWORDS,
  EFFECT_SILENT_ASSASSIN,
  EFFECT_SILENCE_SELF,
  EFFECT_ADJUST_MANA,
  EFFECT_SET_MANA,
  EFFECT_CLEAR_SKIES_SET_MANA,
  EFFECT_PENGUIN_PALS,
  EFFECT_AUDIO_AMPLIFIER,
  EFFECT_DIE,
  EFFECT_SELF_HEAL,
  EFFECT_HEAL_OWN_NEXUS
} EffectType;
typedef struct {
  EffectType type;
  int amount;
} OnPlay;
typedef struct {
  EffectType type;
  int amount;
} OnSummon;
typedef struct {
  EffectType type;
  int amount;
} OnDeath;
typedef struct {
  EffectType type;
  int amount;
} OnAura;
typedef struct {
  EffectType type;
  int amount;
} OnStartTurn;
typedef struct {
  EffectType type;
  int amount;
} OnEndTurn;
extern ECS_COMPONENT_DECLARE(OnPlay);
extern ECS_COMPONENT_DECLARE(OnSummon);
extern ECS_COMPONENT_DECLARE(OnDeath);
extern ECS_COMPONENT_DECLARE(OnAura);
extern ECS_COMPONENT_DECLARE(OnStartTurn);
extern ECS_COMPONENT_DECLARE(OnEndTurn);

// unit tags
extern ECS_TAG_DECLARE(Robot);
extern ECS_TAG_DECLARE(Penguin);

extern ECS_TAG_DECLARE(IsHovering);
extern ECS_TAG_DECLARE(IsDragging);
extern ECS_TAG_DECLARE(CardTag);
extern ECS_TAG_DECLARE(UnitTag);
extern ECS_TAG_DECLARE(PlayerTag);
// extern ECS_TAG_DECLARE(BoardTile);

extern ECS_COMPONENT_DECLARE(MousePosition);
extern ECS_COMPONENT_DECLARE(MouseButtonState);

extern ECS_COMPONENT_DECLARE(Zone);
extern ECS_COMPONENT_DECLARE(ManaCost);
extern ECS_COMPONENT_DECLARE(MousePosition);
extern ECS_COMPONENT_DECLARE(Attack);
extern ECS_COMPONENT_DECLARE(Health);
extern ECS_COMPONENT_DECLARE(CardType);
extern ECS_COMPONENT_DECLARE(Owner);
extern ECS_COMPONENT_DECLARE(Rarity);
extern ECS_COMPONENT_DECLARE(Name);
extern ECS_COMPONENT_DECLARE(ArtPath);
extern ECS_COMPONENT_DECLARE(EffectText);
extern ECS_COMPONENT_DECLARE(Index);
extern ECS_COMPONENT_DECLARE(HandSizes);
extern ECS_COMPONENT_DECLARE(Position);
extern ECS_COMPONENT_DECLARE(Size);
extern ECS_COMPONENT_DECLARE(Rotation);
extern ECS_COMPONENT_DECLARE(Render);
extern ECS_COMPONENT_DECLARE(OnPlay);
extern ECS_COMPONENT_DECLARE(OnSummon);
extern ECS_COMPONENT_DECLARE(OnDeath);
extern ECS_COMPONENT_DECLARE(OnAura);
extern ECS_COMPONENT_DECLARE(OnStartTurn);
extern ECS_COMPONENT_DECLARE(OnEndTurn);

void componentsImport(ecs_world_t *world);

#endif
