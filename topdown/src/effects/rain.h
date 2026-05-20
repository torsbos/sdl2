#ifndef RAIN_H
#define RAIN_H

#include <SDL2/SDL.h>

void rainInit();
void rainUpdate(float deltaTime);
void rainRender(SDL_Renderer *renderer, SDL_Rect *camera);

#define RAIN_COUNT 300
#define SPLASH_COUNT 200
#define LIGHTNING_COOLDOWN 300
#define LIGHTNING_CHANCE 300

#endif
