#pragma once

#include <SDL2/SDL.h>
#include "map.h"

typedef struct {
  Map map;
} Level;

bool levelLoad(Level *level, int id, SDL_Renderer *renderer);
void levelUnload(Level *level);
