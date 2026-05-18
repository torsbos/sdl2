#ifndef PLAYER_H
#define PLAYER_H

#include <stdbool.h>
#include <SDL2/SDL.h>

#define PLAYER_WIDTH  32
#define PLAYER_HEIGHT 64

#define PLAYER_SPEED 150
#define SPRINT_MULTIPLIER 4

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
void playerUpdate(float deltaTime);
void playerRender(SDL_Renderer *renderer, SDL_Rect *camera);
void playerCleanup();

#endif
