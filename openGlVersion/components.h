#ifndef COMPONENTS_H
#define COMPONENTS_H
#include "define.h"
#include "flecs/flecs.h"

// components for cards for ECS
struct ManaCost {
  int manaCost;
};
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
  int isHovering;
} IsHovering;
typedef struct {
  int isDragging;
} IsDragging;

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
  float rotation;
} Rotation;
typedef struct {
  unsigned int vao;
} VAO;
typedef struct {
  unsigned int shaderProgram;
} ShaderProgram;

extern ECS_COMPONENT_DECLARE(ManaCost);
extern ECS_COMPONENT_DECLARE(Attack);
extern ECS_COMPONENT_DECLARE(Health);
extern ECS_COMPONENT_DECLARE(CardType);
extern ECS_COMPONENT_DECLARE(Owner);
extern ECS_COMPONENT_DECLARE(Rarity);
extern ECS_COMPONENT_DECLARE(IsHovering);
extern ECS_COMPONENT_DECLARE(IsDragging);
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
