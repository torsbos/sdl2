#pragma once

#include <SDL2/SDL.h>
#include <stdbool.h>

#define TILE_EMPTY 0
#define TILE_WALL  1

#define OBJECT_PLAYER_SPAWN 100
#define OBJECT_ENEMY_SPAWN 101

SDL_Texture *mapGetTileset(void);

typedef struct Map Map;   // <-- IMPORTANT forward declaration

typedef struct {
  int x;
  int y;
  int type;
} MapObject;

struct Map {
  int width;
  int height;

  int *tiles;
  bool *collision;

  MapObject *objects;
  int objectCount;

  int playerSpawnX;
  int playerSpawnY;
};

// FUNCTIONS
bool mapLoad(SDL_Renderer *renderer);

void mapCleanup();


bool mapIsSolid(Map *map, int x, int y);

bool mapRectCollide(
  Map *map,
  float x,
  float y,
  int w,
  int h
);

void mapRender(Map *map, SDL_Renderer *renderer, SDL_Texture *tileset, SDL_Rect *camera);

int mapGetPlayerSpawnX(void);
int mapGetPlayerSpawnY(void);

Map *mapGetCurrent(void);

void mapBuild(Map *map, int *source, int w, int h, SDL_Renderer *renderer);
