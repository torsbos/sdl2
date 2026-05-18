#ifndef PLAYER_H
#define PLAYER_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include "config.h"
#include "entity.h"


typedef struct {

  float x, y;

  bool moveLeft;
  bool moveRight;
  bool moveUp;
  bool moveDown;

  bool sprinting;
  bool faceLeft;

} Player;

extern Player player;

bool playerLoad(SDL_Renderer *renderer);
void playerSetup(int spawnX, int spawnY);
void playerInput(const Uint8 *state);
void playerUpdate(Entity *e, float deltaTime);
void playerRender(Entity *e, SDL_Renderer *renderer, SDL_Rect *camera);
void playerCleanup();

#endif
