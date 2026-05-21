#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#include "config.h"
#include "player.h"

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

bool shouldQuit = false;
float dt = 0;
Uint64 last = 0;
Uint64 now = 0;

bool initSDL(){

  SDL_Init(SDL_INIT_EVERYTHING);

  window = SDL_CreateWindow(
    TITLE,
    0, //SDL_WINDOWPOS_CENTERED,
    0, //SDL_WINDOWPOS_CENTERED,
    WINDOW_WIDTH,
    WINDOW_HEIGHT,
    SDL_WINDOW_SHOWN
  );
  if (!window) {
    fprintf(stderr, "Error creating window: %s\n", SDL_GetError());
    return true;
  }

  renderer = SDL_CreateRenderer(window, -1, 0);
  if (!renderer) {
    fprintf(stderr, "Error creating renderer: %s\n", SDL_GetError());
    return true;
  }

  return false;
}

void cleanup(){

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

}

void setupLevel(){
  // TODO: 
  // * tilemap
  // * not hardcode level data
  // * entity handler
  // * multiple level support

  playerSetup();

}


void processInput(){
  // TODO:
  // *

  SDL_Event event;
  while (SDL_PollEvent(&event)){
    switch (event.type) {
      case SDL_QUIT:
        shouldQuit = 1;
        break;
      case SDL_KEYUP:
        if (event.key.keysym.sym == SDLK_ESCAPE)
          shouldQuit = 1;
        if (event.key.keysym.sym == SDLK_q)
          shouldQuit = 1;
        break;
    }
  }

  const Uint8 *state = SDL_GetKeyboardState(NULL);
  playerInput(state);

}


void update(){
  // TODO:
  // * framecap
  // * collision
  // * debug text 

  
  now = SDL_GetTicks();
  dt = (now - last) * 0.001f;
  last = now;
  SDL_Log("%f", dt);
  

  playerUpdate(dt);
  
}

void render(){
  // TODO: render player sprite
  // TODO: render tile map
  // TODO: render debug text

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  playerRender(renderer);

  SDL_RenderPresent(renderer);

}

int main(){

  initSDL();

  if (initSDL()) {
    cleanup();
    printf("All bad!!!!\n");
    return 1;
  }
  setupLevel();

  last = SDL_GetTicks();
  while (!shouldQuit){

    processInput();
    update();
    render();

    SDL_Delay(dt);

  } 

  cleanup();
  printf("All good!\n");
  return 0;
}
