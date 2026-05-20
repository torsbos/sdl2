#pragma once

#include <SDL2/SDL.h>
#include "entity.h"

Entity *enemyCreate(float x, float y);

void enemyUpdate(Entity *e, float deltaTime);

void enemyRender(
  Entity *e,
  SDL_Renderer *renderer,
  SDL_Rect *camera
);
