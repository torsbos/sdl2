#ifndef CONFIG_H
#define CONFIG_H

#define TITLE             "game"
#define WINDOW_WIDTH      800
#define WINDOW_HEIGHT     600
#define FPS               60

#define FONT_SIZE         16
#define TILE_SIZE         16
#define TILE_SCALE        4
#define TILE_RENDER_SIZE  (TILE_SIZE * TILE_SCALE)
#define MAP_WIDTH         16
#define MAP_HEIGHT        16
#define WORLD_WIDTH       MAP_WIDTH * TILE_RENDER_SIZE
#define WORLD_HEIGHT      MAP_HEIGHT * TILE_RENDER_SIZE

#endif
