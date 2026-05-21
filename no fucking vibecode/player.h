#ifndef PLAYER_H
#define PLAYER_H

#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "config.h"


void playerSetup();
void playerInput(const Uint8 *state);
void playerUpdate();
void playerRender(SDL_Renderer *renderer);
void playerCleanup();

#endif
