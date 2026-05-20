#include <stdlib.h>
#include "level.h"
#include "map.h"
#include "config.h"
#include "entity.h"
#include "player.h"
#include "enemy.h"


// LEVEL ARRAY

static int level0[] = {
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,100,1,1,1,1,1,1,1,1,1,1,0,101,0,1,1,1,1,1,1,1
};

// FUNCTIONS

bool levelLoad(Level *level, int id, SDL_Renderer *renderer)
{
  int *data = NULL;
  //int w = 16;
  //int h = 16;
  data = level0;

  if (id == 0) {
    data = level0;
  }

  if (!data)
    return false;

  mapBuild(&level->map, data, MAP_WIDTH, MAP_HEIGHT, renderer);


  playerCreate(
    level->map.playerSpawnX,
    level->map.playerSpawnY
  );

  for (int i = 0; i < level->map.objectCount; i++) {

    MapObject *obj = &level->map.objects[i];

    if (obj->type == OBJECT_ENEMY_SPAWN) {

      enemyCreate(obj->x, obj->y);
    }

  }

  return true;
}
