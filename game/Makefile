all:
	gcc \
-Wall \
-std=c99 \
-I./src \
-I./src/core \
-I./src/entities \
-lSDL2 \
./src/core/game.c \
./src/entities/player.c \
-o build/game

run:
	./build/game

clean:
	rm ./build/game
