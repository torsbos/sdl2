#include "player.h"
#include "config.h"

Player player;

void playerSetup(void) {

  player.x = 400;
  player.y = 300;

}

void playerInput(const Uint8 *state) {

  player.moveUp    = state[SDL_SCANCODE_W];
  player.moveDown  = state[SDL_SCANCODE_S];
  player.moveLeft  = state[SDL_SCANCODE_A];
  player.moveRight = state[SDL_SCANCODE_D];

  player.sprinting = state[SDL_SCANCODE_LSHIFT];

}

void playerUpdate(void) {

  float speed = PLAYER_SPEED;

  if (player.sprinting) {
    speed *= SPRINT_MULTIPLIER;
  }

  if (player.moveUp) {
    player.y -= speed;
  }

  if (player.moveDown) {
    player.y += speed;
  }

  if (player.moveLeft) {
    player.faceLeft = true;
    player.x -= speed;
  }

  if (player.moveRight) {
    player.faceLeft = false;
    player.x += speed;
  }

  //border collision

  if (player.y <= 0) {
    player.y = 0;
  }
  if (player.y >= WINDOW_HEIGHT - PLAYER_HEIGHT) {
    player.y = WINDOW_HEIGHT - PLAYER_HEIGHT;
  }
  if (player.x <= 0) {
    player.x = 0;
  }
  if (player.x >= WINDOW_WIDTH - PLAYER_WIDTH) {
    player.x = WINDOW_WIDTH - PLAYER_WIDTH;
  }
}

void playerRender(SDL_Renderer *renderer, SDL_Texture *texture) {
  

  SDL_Rect playerRect = {
    player.x,
    player.y,
    PLAYER_WIDTH,
    PLAYER_HEIGHT
  };

  int frameTwo = (SDL_GetTicks() / 200) % 2;
  int frameFour = (SDL_GetTicks() / 100) % 4;

  SDL_Rect spriteRectIdle = {
    frameTwo * (TILE / 2),
    0,
    (TILE / 2),
    TILE
  };

  SDL_Rect spriteRectWalkX = {
    frameFour * (TILE / 2),
    TILE,
    (TILE / 2),
    TILE
  };
  SDL_Rect spriteRectWalkY = {
    frameTwo * (TILE / 2) + TILE,
    0,
    (TILE / 2),
    TILE
  };

  if (player.moveUp || player.moveDown) {

    SDL_RenderCopy(
      renderer,
      texture,
      &spriteRectWalkY,
      &playerRect
    );
  } else if (player.moveRight) {

    SDL_RenderCopyEx(
      renderer,
      texture,
      &spriteRectWalkX,
      &playerRect,
      0,
      NULL,
      SDL_FLIP_HORIZONTAL
    );

  } else if (player.moveLeft) {

    SDL_RenderCopyEx(
      renderer,
      texture,
      &spriteRectWalkX,
      &playerRect,
      0,
      NULL,
      SDL_FLIP_NONE
    );

  } else if (!player.faceLeft && !player.moveLeft) {

    SDL_RenderCopyEx(
      renderer,
      texture,
      &spriteRectIdle,
      &playerRect,
      0,
      NULL,
      SDL_FLIP_HORIZONTAL
    );
  } else {

    SDL_RenderCopyEx(
      renderer,
      texture,
      &spriteRectIdle,
      &playerRect,
      0,
      NULL,
      SDL_FLIP_NONE
    );
  }
}
