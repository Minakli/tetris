CC = gcc
# CFLAGS = -std=c11 -Wall -Wextra
CFLAGS = -g

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

clang:
	clang-format -n src/brick_game/tetris/*.c
	clang-format -i src/brick_game/tetris/*.c
	clang-format -n src/gui/cli/*.c
	clang-format -i src/gui/cli/*.c

clean:
	rm -rf build
# rm -f brick_game_high_score.txt

install: all
	cp build/tetris /usr/local/bin/
	touch /usr/local/bin/brick_game_high_score.txt

uninstall: clean
	rm /usr/local/bin/tetris
	rm /usr/local/bin/brick_game_high_score.txt