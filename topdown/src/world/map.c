#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

#include "map.h"
#include "config.h"

// STATIC
static int tilesetWidth;
static int tilesetHeight;
static SDL_Texture *tilesetTexture = NULL;
static Map *currentMap = NULL;

// GETTERS
SDL_Texture *mapGetTileset(void)
{
  return tilesetTexture;
}
Map *mapGetCurrent(void)
{
  return currentMap;
}

// FUNCTIONS
bool mapLoad(SDL_Renderer *renderer) {

  SDL_Surface *surface = IMG_Load(ASSET_TEXTURES "tiles.png");
  if (!surface) {
    SDL_Log(
      "Failed to load tiles.png: %s",
      IMG_GetError()
    );
    return false;
  }

  tilesetTexture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);
  if (!tilesetTexture) {
    SDL_Log(
      "Failed to create tileset texture: %s",
      SDL_GetError()
    );
    return false;
  }

  SDL_QueryTexture(
    tilesetTexture,
    NULL,
    NULL,
    &tilesetWidth,
    &tilesetHeight
  );
  return true;
}

void mapBuild(Map *map, int *source, int w, int h, SDL_Renderer *renderer)
{
  map->width = w;
  map->height = h;
  currentMap = map;

  int size = w * h;

  map->tiles = malloc(sizeof(int) * size);
  map->collision = malloc(sizeof(bool) * size);

  map->objects = malloc(sizeof(MapObject) * 256);
  map->objectCount = 0;

  for (int i = 0; i < size; i++) {
    int t = source[i];

    // default
    map->tiles[i] = 0;
    map->collision[i] = false;

    // OBJECTS (logic layer)
    if (t == OBJECT_PLAYER_SPAWN) {
      int x = i % w;
      int y = i / w;

      map->playerSpawnX = x * TILE_RENDER_SIZE;
      map->playerSpawnY = y * TILE_RENDER_SIZE;

      continue;
    }

    if (t == OBJECT_ENEMY_SPAWN) {
      MapObject obj = {
        .x = (i % w) * TILE_RENDER_SIZE,
        .y = (i / w) * TILE_RENDER_SIZE,
        .type = OBJECT_ENEMY_SPAWN
      };

      map->objects[map->objectCount++] = obj;
      continue;
    }

    // TILE LAYER
    map->tiles[i] = t;

    // COLLISION LAYER
    if (t == TILE_WALL) {
      map->collision[i] = true;
    }
  }
}

void mapRender(Map *map, SDL_Renderer *renderer, SDL_Texture *tileset, SDL_Rect *camera)
{
  // int tilesPerRow = 16;
  int tilesPerRow = tilesetWidth / TILE_SIZE;

  for (int y = 0; y < map->height; y++) {
    for (int x = 0; x < map->width; x++) {

      int i = y * map->width + x;
      int tile = map->tiles[i];

      SDL_Rect dst = {
        x * TILE_RENDER_SIZE - camera->x,
        y * TILE_RENDER_SIZE - camera->y,
        TILE_RENDER_SIZE,
        TILE_RENDER_SIZE
      };

      if (dst.x + dst.w < 0 ||
          dst.x > WINDOW_WIDTH ||
          dst.y + dst.h < 0 ||
          dst.y > WINDOW_HEIGHT) {
        continue;
      }

      SDL_Rect src = {
        (tile % tilesPerRow) * TILE_SIZE,
        (tile / tilesPerRow) * TILE_SIZE,
        TILE_SIZE,
        TILE_SIZE
      };

      SDL_RenderCopy(renderer, tileset, &src, &dst);
    }
  }
}

bool mapIsSolid(Map *map, int x, int y)
{
  if (
    x < 0 ||
    y < 0 ||
    x >= map->width * TILE_RENDER_SIZE ||
    y >= map->height * TILE_RENDER_SIZE
  ) {
    return true;
  }

  int tileX = x / TILE_RENDER_SIZE;
  int tileY = y / TILE_RENDER_SIZE;

  int i = tileY * map->width + tileX;

  return map->collision[i];
}

bool mapRectCollide(
  Map *map,
  float x,
  float y,
  int w,
  int h
)
{
  int left = (int)x;
  int right = (int)(x + w - 1);

  int top = (int)y;
  int bottom = (int)(y + h - 1);

  return
    mapIsSolid(map, left, top) ||
    mapIsSolid(map, right, top) ||
    mapIsSolid(map, left, bottom) ||
    mapIsSolid(map, right, bottom);
}

void mapCleanup() {

    if (tilesetTexture) {

        SDL_DestroyTexture(
            tilesetTexture
        );

        tilesetTexture = NULL;
    }
}


