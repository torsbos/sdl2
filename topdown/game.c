#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "player.h"
#include "config.h"
#include "map.h"
#include "rain.h"

// TODO:

// * only render tiles on screen
// * enemy entity
// * entity handler
// * game objectives
// * missions
// * menu, titlescreen 
// * level select
// * spawn player and enemies through map files
// * only render tiles on screen
// * collision detection, on other objects 
// * level editor

// COMPLETE:

// * player movement
// * player animation
// * tilemap generation
// * deltatime
// * rain engine
// * debug text with player x,y and fps

// GLOBAL DECLARATIONS

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

TTF_Font *textFont = NULL;
SDL_Texture *textTexture = NULL;
SDL_Rect textRect;
SDL_Color textColor = {255,255,255,255};

SDL_Rect camera = {
    0,
    0,
    WINDOW_WIDTH,
    WINDOW_HEIGHT
};

int worldWidth = MAP_WIDTH * TILE_RENDER_SIZE;
int worldHeight = MAP_HEIGHT * TILE_RENDER_SIZE;

bool shouldQuit = false;
bool showDebug = false;

float deltaTime = 0.0f;
Uint32 lastFrameTime = 0;

float currentFPS = 0.0f;
float fpsTimer = 0.0f;
int fpsFrames = 0;

const int targetFrameTime = 1000 / FPS;

// FUNCTIONS

bool initSDL(){

  if (SDL_Init(SDL_INIT_EVERYTHING)) {
    fprintf(stderr, "Error initializing SDL: %s\n", SDL_GetError());
    return true;
  }

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

  if (TTF_Init()) {
    fprintf(stderr, "Error initializing TTF: %s\n", TTF_GetError());
    return true;
  }

  textFont = TTF_OpenFont("regular.ttf", FONT_SIZE);
  if (!textFont) {
    fprintf(stderr, "Error loading font: %s\n", TTF_GetError());
    return true;
  }

  if (IMG_Init(IMG_INIT_PNG) < 0)
  {
    fprintf(stderr, "Error initializing IMG: %s\n", IMG_GetError());
    return true;
  }

  if (!mapLoad(renderer)) {
    return true;
  }

  if (!playerLoad(renderer)) {
    return true;
  }

  return false;
}

void updateFPS() {

  fpsTimer += deltaTime;
  fpsFrames++;

  if (fpsTimer >= 0.5f) {

    currentFPS = fpsFrames / fpsTimer;

    fpsFrames = 0;
    fpsTimer = 0.0f;
  }
}

void updateDebugText() {

    char buffer[128];

    snprintf(
        buffer,
        sizeof(buffer),
        "X: %.0f Y: %.0f FPS: %.0f",
        player.x,
        player.y,
        currentFPS
    );

    SDL_Surface* textSurface =
      TTF_RenderText_Solid(textFont, buffer, textColor);

    if (!textSurface) {
      fprintf(stderr, "Text surface error: %s\n", TTF_GetError());
      return;
    }

    SDL_Texture* newTextTexture =
      SDL_CreateTextureFromSurface(renderer, textSurface);

    if (!newTextTexture) {
      fprintf(stderr, "Texture error: %s\n", SDL_GetError());
      SDL_FreeSurface(textSurface);
      return;
    }

    if (textTexture) {
      SDL_DestroyTexture(textTexture);
    }

    textTexture = newTextTexture;

    textRect.x = 20;
    textRect.y = 20;
    textRect.w = textSurface->w;
    textRect.h = textSurface->h;

    SDL_FreeSurface(textSurface);
}

void cleanup(){


  TTF_CloseFont(textFont);
  SDL_DestroyTexture(textTexture);

  playerCleanup();
  mapCleanup();

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  TTF_Quit();
  IMG_Quit();
  SDL_Quit();

}

void setupLevel(){

  playerSetup();

  rainInit();

}


void processInput(){

  SDL_Event event;
  while (SDL_PollEvent(&event)){
    switch (event.type) {
      case SDL_QUIT:
        shouldQuit = true;
        break;
      case SDL_KEYUP:
        if (event.key.keysym.sym == SDLK_ESCAPE)
          shouldQuit = true;
        if (event.key.keysym.sym == SDLK_q)
          shouldQuit = true;
        break;
      case SDL_KEYDOWN:
        if (!event.key.repeat)
          if (event.key.keysym.sym == SDLK_n)
            showDebug = !showDebug;
    }
  }
  const Uint8 *state = SDL_GetKeyboardState(NULL);

  playerInput(state);
}

void cameraUpdate(){

  //camera follow player, dont scroll past edge
  camera.x = (int)(player.x - WINDOW_WIDTH / 2);
  camera.y = (int)(player.y - WINDOW_HEIGHT / 2);

  if (camera.x < 0) {
    camera.x = 0;
  }
  if (camera.y < 0) {
    camera.y = 0;
  }
  if (camera.x > worldWidth - WINDOW_WIDTH) {
    camera.x = worldWidth - WINDOW_WIDTH;
  }
  if (camera.y > worldHeight - WINDOW_HEIGHT) {
    camera.y = worldHeight - WINDOW_HEIGHT;
  }

  if (worldWidth < WINDOW_WIDTH)
    camera.x = 0;

  if (worldHeight < WINDOW_HEIGHT)
    camera.y = 0;
}

void update(){


  playerUpdate(deltaTime); 

  cameraUpdate();

  rainUpdate(deltaTime);
 
  updateFPS();

  if (showDebug) { updateDebugText(); }
  if (!showDebug && textTexture) {
    SDL_DestroyTexture(textTexture);
    textTexture = NULL;
  }
}

void render(){

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
  SDL_RenderClear(renderer);

  mapRender(renderer, &camera);

  playerRender(renderer, &camera);

  rainRender(renderer);

  if (showDebug) { 
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
  }


  SDL_RenderPresent(renderer);

}

void calcDeltaTime(){

  Uint32 currentTime = SDL_GetTicks();

  deltaTime = (currentTime - lastFrameTime) / 1000.0f;
  lastFrameTime = currentTime;

  if (deltaTime > 0.0f) {
    currentFPS = 1.0f / deltaTime;
  }

}

void waitDeltaTime(Uint32 frameStart){

  Uint32 frameTime = SDL_GetTicks() - frameStart;
  if (frameTime < targetFrameTime) {
    SDL_Delay(targetFrameTime - frameTime);
  }

}

int main(){

  srand(time(NULL));

  if (initSDL()) { 
    cleanup();
    return 1;
  }

  setupLevel();

  lastFrameTime = SDL_GetTicks();

  while (!shouldQuit) {

    Uint32 frameStart = SDL_GetTicks();

    calcDeltaTime();

    processInput();
    update();
    render();

    waitDeltaTime(frameStart);
  }
  cleanup();
  printf("All good!\n");
  return 0;
}
