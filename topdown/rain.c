#include <stdlib.h>
#include <SDL2/SDL.h>
#include <stdbool.h>

#include "rain.h"
#include "config.h"


typedef struct {

  float x;
  float y;

  float speed;

  int length;

  float targetY;

} RainDrop;

typedef struct {

  float x;
  float y;

  float vx;
  float vy;

  int life;

} SplashParticle;

static RainDrop rain[RAIN_COUNT];
static SplashParticle splashes[SPLASH_COUNT];

static void createSplash(float x, float y) {

  for (int i = 0; i < SPLASH_COUNT; i++) {

    if (splashes[i].life <= 0) {

      splashes[i].x = x;
      splashes[i].y = y;

      splashes[i].vx =
          rand() % 2;

      splashes[i].vy =
          -(1 + rand() % 1);

      splashes[i].life = 20;

      break;
    }
  }
}

void rainInit() {

  for (int i = 0; i < RAIN_COUNT; i++) {

    rain[i].x = rand() % WINDOW_WIDTH;
    rain[i].y = rand() % WINDOW_HEIGHT;

    rain[i].speed = 200 + rand() % 400;
    rain[i].length = 5 + rand() % 10;

    rain[i].targetY = 100 + rand() % (WINDOW_HEIGHT - 100);
  }
}

static void updateSplashes() {

  for (int i = 0; i < SPLASH_COUNT; i++) {

    if (splashes[i].life > 0) {

      splashes[i].x += splashes[i].vx;
      splashes[i].y += splashes[i].vy;

      splashes[i].vy += 0.05f;

      splashes[i].life--;
    }
  }
}

void rainUpdate(float deltaTime) {

  for (int i = 0; i < RAIN_COUNT; i++) {

    rain[i].y += rain[i].speed * deltaTime;

    //wind
    rain[i].x -= 0.8f;

    if (rain[i].x < 0) {
      rain[i].x = WINDOW_WIDTH;
    }

    if (rain[i].y >= rain[i].targetY) {

      for (int s = 0; s < 5; s++) {

        createSplash(
          rain[i].x,
          rain[i].y
        );
      }

      rain[i].x = rand() % WINDOW_WIDTH;
      rain[i].y = -(rand() % WINDOW_HEIGHT);

      rain[i].targetY = 100 + rand() % (WINDOW_HEIGHT - 100);
    }
  }
  updateSplashes();

}

static void renderSplashes(SDL_Renderer *renderer) {

  SDL_SetRenderDrawColor(
    renderer,
    255,
    255,
    255,
    255
  );

  for (int i = 0; i < SPLASH_COUNT; i++) {

    if (splashes[i].life > 0) {

      SDL_Rect rect = {
        (int)splashes[i].x,
        (int)splashes[i].y,
        2,
        2
      };

      SDL_RenderFillRect(renderer, &rect);
    }
  }
}

void rainRender(SDL_Renderer *renderer) {

  SDL_SetRenderDrawColor(
    renderer,
    255,
    255,
    255,
    255
  );

  for (int i = 0; i < RAIN_COUNT; i++) {

    SDL_RenderDrawLine(

      renderer,

      (int)rain[i].x,
      (int)rain[i].y,

      (int)rain[i].x - 2,
      (int)(rain[i].y + rain[i].length)
    );
  }

  renderSplashes(renderer);
}
