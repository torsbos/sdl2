#pragma once

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "config.h"

typedef struct Entity Entity;
extern Entity *playerEntity;

typedef void (*EntityUpdateFn)(Entity *e, float deltaTime);
typedef void (*EntityRenderFn)(Entity *e, SDL_Renderer *renderer, SDL_Rect *camera);

typedef enum {
  ENTITY_PLAYER,
  ENTITY_ENEMY
} EntityType;

struct Entity {
  float x, y;
  float vx, vy;

  int width;
  int height;

  bool active;

  EntityType type;

  void *data;

  EntityUpdateFn update;
  EntityRenderFn render;
};


void entityInit(void);
Entity *entityCreate(void);
void entityUpdateAll(float deltaTime);
void entityRenderAll(SDL_Renderer *renderer, SDL_Rect *camera);
