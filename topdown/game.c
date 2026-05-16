#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "player.h"
#include "config.h"
#include "rain.h"

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

TTF_Font *textFont = NULL;
SDL_Texture *textTexture = NULL;
SDL_Rect textRect;
SDL_Color textColor = {255,255,255,255};

SDL_Texture *spriteTexture = NULL;
SDL_Surface *spriteSurface = NULL;


bool shouldQuit = false;
bool showDebug = false;

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

  spriteSurface = IMG_Load("sprite.png");
  if (!spriteSurface) {
    fprintf(stderr, "Error loading sprite to surface: %s\n", IMG_GetError());
    return true;
  }

  spriteTexture = 
    SDL_CreateTextureFromSurface(renderer, spriteSurface);
  if (!spriteTexture) {
    fprintf(stderr, "Error creating sprite texture from surface: %s\n", IMG_GetError());
    return true;
  }

  SDL_FreeSurface(spriteSurface);

return false;
}

void updateDebugText() {

    char buffer[128];

    snprintf(
        buffer,
        sizeof(buffer),
        "X: %.0f  Y: %.0f",
        player.x,
        player.y
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
  SDL_DestroyTexture(spriteTexture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  TTF_Quit();
  IMG_Quit();
  SDL_Quit();

}

void setupLevel(){
  // TODO: add tilemap?

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

void update(){
  // TODO: collision detection, on other objects 
  // collision on tilemap?
  // deltatime
  // weather effect


  //player movement and collision

  playerUpdate(); 

  rainUpdate();

 
  // debug text
  if (showDebug) { updateDebugText(); }
  if (!showDebug && textTexture) {
    SDL_DestroyTexture(textTexture);
    textTexture = NULL;
  }

  SDL_Delay(1000 / FPS);
}

void render(){
  // TODO: render tile map
  // weather effect

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
  SDL_RenderClear(renderer);

  playerRender(renderer, spriteTexture);

  rainRender(renderer);

  if (showDebug) { 
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
  }


  SDL_RenderPresent(renderer);

}

int main(){

  srand(time(NULL));

  if (initSDL()) { 
    cleanup();
    return 1;
  }

  setupLevel();

  while (!shouldQuit){
  
    processInput();
    update();
    render();

  } 

  cleanup();
  printf("All good!\n");
  return 0;
}
