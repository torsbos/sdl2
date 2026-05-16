#ifndef PLAYER_H
#define PLAYER_H

#include <stdbool.h>
#include <SDL2/SDL.h>

#define PLAYER_WIDTH  32
#define PLAYER_HEIGHT 64

#define PLAYER_SPEED 1.8f
#define SPRINT_MULTIPLIER 1.4f

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
void playerSetup();
void playerInput(const Uint8 *state);
void playerUpdate();
void playerRender(SDL_Renderer *renderer);
void playerCleanup();

#endif
