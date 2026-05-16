#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

#include "map.h"
#include "config.h"

static SDL_Texture *tilesetTexture = NULL;

static int tilesetWidth;
static int tilesetHeight;

static int map[MAP_WIDTH * MAP_HEIGHT] = {

  2,2,2,2,2,2,2,2,2,2,
  2,2,2,2,2,2,2,2,2,2,
  2,2,2,2,2,2,2,2,2,2,
  2,2,2,2,2,2,2,2,2,2,
  2,2,2,2,2,2,2,2,2,2,

  2,2,2,2,2,2,2,2,2,2,
  2,2,2,2,2,2,2,2,2,2,
  2,2,2,2,2,2,2,2,2,2,
  2,2,2,2,2,2,2,2,2,2,
  2,2,2,2,2,2,2,2,2,2,
};

bool mapLoad(SDL_Renderer *renderer) {

    SDL_Surface *surface =
        IMG_Load("tiles.png");

    if (!surface) {

        SDL_Log(
            "Failed to load tiles.png: %s",
            IMG_GetError()
        );

        return false;
    }

    tilesetTexture =
        SDL_CreateTextureFromSurface(
            renderer,
            surface
        );

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

void mapRender(SDL_Renderer *renderer) {

    int tilesPerRow =
        tilesetWidth / TILE_SIZE;

    for (int y = 0; y < MAP_HEIGHT; y++) {

        for (int x = 0; x < MAP_WIDTH; x++) {

            int tileID =
                map[y * MAP_WIDTH + x];

            SDL_Rect src = {

                (tileID % tilesPerRow) * TILE_SIZE,

                (tileID / tilesPerRow) * TILE_SIZE,

                TILE_SIZE,
                TILE_SIZE
            };

            SDL_Rect dst = {

                x * TILE_RENDER_SIZE,
                y * TILE_RENDER_SIZE,

                TILE_RENDER_SIZE,
                TILE_RENDER_SIZE
            };

            SDL_RenderCopy(
                renderer,
                tilesetTexture,
                &src,
                &dst
            );
        }
    }
}

void mapCleanup() {

    if (tilesetTexture) {

        SDL_DestroyTexture(
            tilesetTexture
        );

        tilesetTexture = NULL;
    }
}


