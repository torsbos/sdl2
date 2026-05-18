#include <SDL2/SDL_image.h>
#include "config.h"
#include "map.h"
#include "entity.h"
#include "player.h"

SDL_Texture *texture = NULL;

Player player;


typedef struct {
  float speed;
} PlayerData;

// FUNCTIONS
void playerSetup(int spawnX, int spawnY)
{
  player.x = spawnX;
  player.y = spawnY;
}

void playerInput(const Uint8 *state) {

  player.moveUp    = state[SDL_SCANCODE_W];
  player.moveDown  = state[SDL_SCANCODE_S];
  player.moveLeft  = state[SDL_SCANCODE_A];
  player.moveRight = state[SDL_SCANCODE_D];

  player.sprinting = state[SDL_SCANCODE_LSHIFT];

}

void playerUpdate(Entity *e, float deltaTime) {

  float speed = PLAYER_SPEED;

  if (player.moveLeft) e->vx = -speed;
  else if (player.moveRight) e->vx = speed;
  else e->vx = 0;

  if (player.moveUp) e->vy = -speed;
  else if (player.moveDown) e->vy = speed;
  else e->vy = 0;

  e->x += e->vx * deltaTime;
  e->y += e->vy * deltaTime;

  //border collision

  if (e->x < 0)
    e->x = 0;

  if (e->y < 0)
    e->y = 0;

  if (e->x > MAP_WIDTH * TILE_RENDER_SIZE - PLAYER_WIDTH)
    e->x = MAP_WIDTH * TILE_RENDER_SIZE - PLAYER_WIDTH;

  if (e->y > MAP_HEIGHT * TILE_RENDER_SIZE - PLAYER_HEIGHT)
    e->y = MAP_HEIGHT * TILE_RENDER_SIZE - PLAYER_HEIGHT;

}

void playerRender(Entity *e, SDL_Renderer *renderer, SDL_Rect *camera) {
  

  SDL_Rect playerRect = {
    (int) (e->x - camera->x),
    (int) (e->y - camera->y),
    PLAYER_WIDTH,
    PLAYER_HEIGHT
  };

  int frameTwo = (SDL_GetTicks() / 200) % 2;
  int frameFour = (SDL_GetTicks() / 100) % 4;

  SDL_Rect spriteRectIdle = {
    frameTwo * (TILE_SIZE / 2),
    0,
    (TILE_SIZE / 2),
    TILE_SIZE
  };

  SDL_Rect spriteRectWalkX = {
    frameFour * (TILE_SIZE / 2) + (TILE_SIZE * 2),
    0,
    (TILE_SIZE / 2),
    TILE_SIZE
  };
  SDL_Rect spriteRectWalkY = {
    frameTwo * (TILE_SIZE / 2) + TILE_SIZE,
    0,
    (TILE_SIZE / 2),
    TILE_SIZE
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
void playerCleanup() {

    if (texture) {

        SDL_DestroyTexture(
            texture
        );

        texture = NULL;
    }
}

bool playerLoad(SDL_Renderer *renderer) {

    SDL_Surface *surface =
        IMG_Load("player.png");

    if (!surface) {

        SDL_Log(
            "Failed to load player.png: %s",
            IMG_GetError()
        );

        return false;
    }

    texture =
        SDL_CreateTextureFromSurface(
            renderer,
            surface
        );

    SDL_FreeSurface(surface);

    if (!texture) {

        SDL_Log(
            "Failed to create player texture: %s",
            SDL_GetError()
        );

        return false;
    }
    return true;
}
