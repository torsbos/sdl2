#ifndef PLAYER_H
#define PLAYER_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include "config.h"
#include "entity.h"


typedef struct {

  float speed;

  bool moveLeft;
  bool moveRight;
  bool moveUp;
  bool moveDown;

  bool faceLeft;
  bool faceRight;
  bool faceUp;
  bool faceDown;

} PlayerData;

Entity *playerCreate(float x, float y);
bool playerLoad(SDL_Renderer *renderer);
void playerInput(Entity *e, const Uint8 *state);
void playerUpdate(Entity *e, float deltaTime);
void playerRender(Entity *e, SDL_Renderer *renderer, SDL_Rect *camera);
void playerCleanup();

#endif
