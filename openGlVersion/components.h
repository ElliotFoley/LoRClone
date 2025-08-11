#ifndef COMPONENTS_H
#define COMPONENTS_H
#include "define.h"
#include "flecs/flecs.h"

// components for cards for ECS
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
  float x, y;
} Position;
typedef struct {
  float width, height;
} Size;
typedef struct {
  float angle;
} Rotation;
typedef struct {
  unsigned int vao;
} VAO;
typedef struct {
  unsigned int shaderProgram;
} ShaderProgram;

typedef struct {
  float x, y;
} MousePosition;

typedef struct {
  int leftDown;
} MouseButtonState;

extern ECS_TAG_DECLARE(IsHovering);
extern ECS_TAG_DECLARE(IsDragging);
extern ECS_TAG_DECLARE(CardTag);
extern ECS_TAG_DECLARE(UnitTag);
extern ECS_TAG_DECLARE(PlayerTag);
// extern ECS_TAG_DECLARE(BoardTile);

extern ECS_COMPONENT_DECLARE(MousePosition);
extern ECS_COMPONENT_DECLARE(MouseButtonState);

extern ECS_COMPONENT_DECLARE(ManaCost);
extern ECS_COMPONENT_DECLARE(Attack);
extern ECS_COMPONENT_DECLARE(Health);
extern ECS_COMPONENT_DECLARE(CardType);
extern ECS_COMPONENT_DECLARE(Owner);
extern ECS_COMPONENT_DECLARE(Rarity);
extern ECS_COMPONENT_DECLARE(Name);
extern ECS_COMPONENT_DECLARE(ArtPath);
extern ECS_COMPONENT_DECLARE(EffectText);
extern ECS_COMPONENT_DECLARE(Position);
extern ECS_COMPONENT_DECLARE(Size);
extern ECS_COMPONENT_DECLARE(Rotation);
extern ECS_COMPONENT_DECLARE(VAO);
extern ECS_COMPONENT_DECLARE(ShaderProgram);

void componentsImport(ecs_world_t *world);

#endif
