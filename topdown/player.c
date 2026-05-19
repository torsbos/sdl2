#include <SDL2/SDL_image.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "map.h"
#include "entity.h"
#include "player.h"

SDL_Texture *texture = NULL;

// FUNCTIONS

Entity *playerCreate(float x, float y)
{
  Entity *e = entityCreate();

  if (!e)
      return NULL;

  PlayerData *pd = malloc(sizeof(PlayerData));

  memset(pd, 0, sizeof(PlayerData));

  pd->speed = PLAYER_SPEED;

  e->x = x;
  e->y = y;

  e->width = PLAYER_WIDTH;
  e->height = PLAYER_HEIGHT;

  e->data = pd;

  e->update = playerUpdate;
  e->render = playerRender;

  playerEntity = e;

  return e;
}

void playerInput(Entity *e, const Uint8 *state) {

  PlayerData *pd = e->data;

  pd->moveUp    = state[SDL_SCANCODE_W];
  pd->moveDown  = state[SDL_SCANCODE_S];
  pd->moveLeft  = state[SDL_SCANCODE_A];
  pd->moveRight = state[SDL_SCANCODE_D];

}

void playerUpdate(Entity *e, float deltaTime) {

  PlayerData *pd = e->data;

  if (pd->moveLeft) e->vx = -PLAYER_SPEED;
  else if (pd->moveRight) e->vx = PLAYER_SPEED;
  else e->vx = 0;

  if (pd->moveUp) e->vy = -PLAYER_SPEED;
  else if (pd->moveDown) e->vy = PLAYER_SPEED;
  else e->vy = 0;

  Map *map = mapGetCurrent();

  float nextX = e->x + e->vx * deltaTime;

  if (!mapRectCollide(
    map,
    nextX,
    e->y,
    e->width,
    e->height
  )) {
    e->x = nextX;
  }

  float nextY = e->y + e->vy * deltaTime;

  if (!mapRectCollide(
    map,
    e->x,
    nextY,
    e->width,
    e->height
  )) {
    e->y = nextY;
  }

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

  PlayerData *pd = e->data;

  if (pd->moveUp || pd->moveDown) {

    SDL_RenderCopy(
      renderer,
      texture,
      &spriteRectWalkY,
      &playerRect
    );
  } else if (pd->moveRight) {

    SDL_RenderCopyEx(
      renderer,
      texture,
      &spriteRectWalkX,
      &playerRect,
      0,
      NULL,
      SDL_FLIP_HORIZONTAL
    );

  } else if (pd->moveLeft) {

    SDL_RenderCopyEx(
      renderer,
      texture,
      &spriteRectWalkX,
      &playerRect,
      0,
      NULL,
      SDL_FLIP_NONE
    );

  } else if (!pd->faceLeft && !pd->moveLeft) {

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
