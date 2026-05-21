#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#include "config.h"
#include "player.h"

struct player {

  float x, y, vx, vy;
  bool moveLeft, moveRight, moveUp, moveDown;
  bool faceLeft, faceRight, faceUp, faceDown;

} player;

void playerSetup(){

  player.x = 40;
  player.y = 40;

}

void playerInput(const Uint8 *state){

  player.moveUp = state[SDL_SCANCODE_W];
  player.moveDown = state[SDL_SCANCODE_S];
  player.moveLeft = state[SDL_SCANCODE_A];
  player.moveRight = state[SDL_SCANCODE_D];

}

void playerUpdate() {

  //movement 
  player.vx = 0;
  player.vy = 0;

  if (player.moveLeft) {
    player.faceLeft = true;
    player.faceRight = false;

    player.vx = -PLAYER_SPEED;
  }
  if (player.moveRight) {
    player.faceLeft = false;
    player.faceRight = true;

    player.vx = +PLAYER_SPEED;
  }
  if (player.moveUp) {
    player.faceUp = true;
    player.faceDown = false;

    player.vy = -PLAYER_SPEED;
  }
  if (player.moveDown) {
    player.faceUp = false;
    player.faceDown = true;

    player.vy = +PLAYER_SPEED;
  }

  player.x += player.vx;
  player.y += player.vy;
  
  //border collision

  if (player.y <= 0) {
      player.y = 0;
  }

  if (player.x <= 0) {
      player.x = 0;
  }
  if (player.x >= WINDOW_WIDTH - PLAYER_WIDTH) {
      player.x = WINDOW_WIDTH - PLAYER_WIDTH;
  }
  if (player.y >= WINDOW_HEIGHT - PLAYER_HEIGHT) {
      player.y = WINDOW_HEIGHT - PLAYER_HEIGHT;
  }

}
void playerRender(SDL_Renderer *renderer) {

  SDL_Rect player_rect = {
    player.x,
    player.y,
    PLAYER_WIDTH,
    PLAYER_HEIGHT
  };

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderFillRect(renderer, &player_rect);

}

void playerCleanup() {}
