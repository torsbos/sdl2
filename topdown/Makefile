.PHONY: all clean

all:
	gcc -Wall \
  -I./src \
  -I./src/core \
  -I./src/entities \
  -I./src/world \
  -I./src/effects \
  -std=c99 \
  -lSDL2 -lSDL2_ttf -lSDL2_image \
  src/core/game.c \
  src/entities/player.c \
  src/entities/enemy.c \
  src/world/map.c \
  src/effects/rain.c \
  src/world/level.c \
  src/core/entity.c \
  -o ./build/game

run:
	./build/game

clean:
	rm ./build/game

