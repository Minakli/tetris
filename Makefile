CC = gcc
# CFLAGS = -std=c11 -Wall -Wextra
CFLAGS = -std=c11

SRC_UI = $(shell find src/gui/cli -name "*.c")
SRC_TETRIS = $(shell find src/brick_game/tetris/ -name "*.c")

OBJ_UI = $(patsubst src/gui/cli/%.c, build/gui/%.o, $(SRC_UI))
OBJ_GAME = $(patsubst src/brick_game/tetris/%.c, build/brick_game/tetris/%.o, $(SRC_TETRIS))

all: game

game: tetris.a ui.o
	$(CC) $(CFLAGS) build/gui/ui.o build/brick_game/tetris/tetris.a -o build/tetris -lncurses

tetris.a: $(OBJ_GAME)
	ar rcs build/brick_game/tetris/tetris.a $(OBJ_GAME)

ui.o: $(OBJ_UI)
	$(CC) -r $(CFLAGS) $(OBJ_UI) -o build/gui/ui.o

build/gui/%.o: src/gui/cli/%.c
	mkdir -p build/gui/
	$(CC) $(CFLAGS) -c $< -o $@

build/brick_game/tetris/%.o: src/brick_game/tetris/%.c
	mkdir -p build/brick_game/tetris/
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf build

install: all
	cp build/tetris /usr/local/bin/

uninstall: clean
	rm /usr/local/bin/tetris
