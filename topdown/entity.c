#include "entity.h"

static Entity entities[MAX_ENTITIES];

Entity *playerEntity = NULL;

//FUNCTIONS
void entityInit(void)
{
  for (int i = 0; i < MAX_ENTITIES; i++)
    entities[i].active = false;
}

Entity *entityCreate(void)
{
  for (int i = 0; i < MAX_ENTITIES; i++)
  {
    if (!entities[i].active)
    {
      Entity *e = &entities[i];

      e->active = true;
      e->x = 0;
      e->y = 0;
      e->vx = 0;
      e->vy = 0;
      e->update = NULL;
      e->render = NULL;

      // assign player reference
      if (playerEntity == NULL)
        playerEntity = e;

      return e;
    }
  }

  return NULL;
}

void entityUpdateAll(float dt)
{
  for (int i = 0; i < MAX_ENTITIES; i++)
  {
    if (entities[i].active && entities[i].update)
      entities[i].update(&entities[i], dt);
  }
}

void entityRenderAll(SDL_Renderer *renderer, SDL_Rect *camera)
{
  for (int i = 0; i < MAX_ENTITIES; i++)
  {
    if (entities[i].active && entities[i].render)
      entities[i].render(&entities[i], renderer, camera);
  }
}
