#include <stdlib.h>
#include "level.h"
#include "map.h"
#include "config.h"


// LEVEL ARRAY

static int level0[] = {
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,100,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
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

  //mapBuild(&level->map, data, w, h, renderer);
  mapBuild(&level->map, data, MAP_WIDTH, MAP_HEIGHT, renderer);

  return true;
}
