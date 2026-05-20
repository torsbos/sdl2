#include <stdlib.h>
#include <SDL2/SDL.h>

#include "enemy.h"
#include "map.h"
#include "config.h"
#include "player.h"

//TYPEDEF
typedef enum {
  ENEMY_IDLE,
  ENEMY_PATROL,
  ENEMY_CHASE
} EnemyState;

typedef struct {
  float speed;
  int direction;
  EnemyState state;
  float idleTimer;
  float visionRange;
  float chaseSpeed;
} EnemyData;

//HELPERS
static bool rectOverlap(
  float x1,
  float y1,
  int w1,
  int h1,
  float x2,
  float y2,
  int w2,
  int h2
)
{
  return
    x1 < x2 + w2 &&
    x1 + w1 > x2 &&
    y1 < y2 + h2 &&
    y1 + h1 > y2;
}

static float absf(float x)
{
  return x < 0 ? -x : x;
}

// FUNCTIONS
Entity *enemyCreate(float x, float y)
{
  Entity *e = entityCreate();

  if (!e) {
    return NULL;
  }

  EnemyData *data = malloc(sizeof(EnemyData));

  data->state = ENEMY_PATROL;
  data->idleTimer = 0.0f;

  data->visionRange = 250.0f;
  data->chaseSpeed = 120.0f;

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

  if (playerEntity) {

    float dx =
      playerEntity->x - e->x;

    float dy =
      playerEntity->y - e->y;

    if (
      absf(dx) < data->visionRange &&
      absf(dy) < 80
    ) {

      data->state = ENEMY_CHASE;
    }
  }

  Map *map = mapGetCurrent();

  switch (data->state) {

    case ENEMY_IDLE:

      e->vx = 0;

      data->idleTimer -= deltaTime;

      if (data->idleTimer <= 0) {

        data->state = ENEMY_PATROL;
      }

      break;

    case ENEMY_PATROL:

      e->vx =
        data->speed * data->direction;

      break;

    case ENEMY_CHASE:

      if (!playerEntity) {
        break;
      }

      float dx =
        playerEntity->x - e->x;

      if (absf(dx) > data->visionRange * 1.5f) {

        data->state = ENEMY_PATROL;

        break;
      }

      if (dx > 0) {

        data->direction = 1;

      } else {

        data->direction = -1;
      }

      e->vx =
        data->chaseSpeed * data->direction;

      break;

  }

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

    data->state = ENEMY_IDLE;

    data->idleTimer = 1.0f;

  }

  if (
    playerEntity &&
    rectOverlap(
      e->x,
      e->y,
      e->width,
      e->height,

      playerEntity->x,
      playerEntity->y,
      playerEntity->width,
      playerEntity->height
    )
  ) {

    SDL_Log("Enemy touched player");
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
