#ifndef PLAYER_H
#define PLAYER_H

#include <stdbool.h>
#include <SDL2/SDL.h>

#define PLAYER_WIDTH  32
#define PLAYER_HEIGHT 64

#define PLAYER_SPEED 2.4f
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

void playerSetup(void);
void playerInput(const Uint8 *state);
void playerUpdate(void);
void playerRender(SDL_Renderer *renderer, SDL_Texture *texture);

#endif
