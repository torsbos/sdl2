#include <stdlib.h>
#include <SDL2/SDL.h>

#include "enemy.h"
#include "map.h"
#include "config.h"

typedef struct {
  float speed;
  int direction;
} EnemyData;

Entity *enemyCreate(float x, float y)
{
  Entity *e = entityCreate();

  if (!e) {
    return NULL;
  }

  EnemyData *data = malloc(sizeof(EnemyData));

  data->speed = 50.0f;
  data->direction = 1;

  e->x = x;
  e->y = y;

  e->width = PLAYER_WIDTH;
  e->height = PLAYER_HEIGHT;

  e->type = ENTITY_ENEMY;

  e->data = data;

  e->update = enemyUpdate;
  e->render = enemyRender;

  return e;
}

void enemyUpdate(Entity *e, float deltaTime)
{
  EnemyData *data = e->data;

  Map *map = mapGetCurrent();

  e->vx = data->speed * data->direction;

  float nextX = e->x + e->vx * deltaTime;

  if (!mapRectCollide(
    map,
    nextX,
    e->y,
    e->width,
    e->height
  )) {

    e->x = nextX;

  } else {

    data->direction *= -1;
  }
}

void enemyRender(
  Entity *e,
  SDL_Renderer *renderer,
  SDL_Rect *camera
)
{
  SDL_Rect rect = {
    (int)(e->x - camera->x),
    (int)(e->y - camera->y),
    e->width,
    e->height
  };

  SDL_SetRenderDrawColor(
    renderer,
    255,
    0,
    0,
    255
  );

  SDL_RenderFillRect(renderer, &rect);
}
