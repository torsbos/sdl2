#ifndef MAP_H
#define MAP_H

#include <SDL2/SDL.h>
#include <stdbool.h>

bool mapLoad(SDL_Renderer *renderer);

void mapRender(SDL_Renderer *renderer, SDL_Rect *camera);

void mapCleanup();

#endif
