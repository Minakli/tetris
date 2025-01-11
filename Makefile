CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Werror
CFLAGS_TEST = --coverage

SRC_UI = $(shell find src/gui/cli -name "*.c")
SRC_TETRIS = $(shell find src/brick_game/tetris/ -name "*.c")
SRC_TEST = $(shell find src/tests -name "*.c")

OBJ_UI = $(patsubst src/gui/cli/%.c, build/gui/%.o, $(SRC_UI))
OBJ_GAME = $(patsubst src/brick_game/tetris/%.c, build/brick_game/tetris/%.o, $(SRC_TETRIS))
OBJ_TEST = $(patsubst src/tests/%.c, build/tests/%.o, $(SRC_TEST))

OBJ_GCOV = $(patsubst src/brick_game/tetris/%.c, build/brick_game/tetris/%_gcov.o, $(SRC_TETRIS))

# all
all: game

game: tetris.a ui.o
	$(CC) $(CFLAGS) build/gui/ui.o build/brick_game/tetris/tetris.a -o build/tetris -lncurses

# ui
ui.o: $(OBJ_UI)
	$(CC) -r $(CFLAGS) $(OBJ_UI) -o build/gui/ui.o

build/gui/%.o: src/gui/cli/%.c
	mkdir -p build/gui/
	$(CC) $(CFLAGS) -c $< -o $@

# tetris
tetris.a: $(OBJ_GAME)
	ar rcs build/brick_game/tetris/tetris.a $(OBJ_GAME)

build/brick_game/tetris/%.o: src/brick_game/tetris/%.c
	mkdir -p build/brick_game/tetris/
	$(CC) $(CFLAGS) -c $< -o $@

# test
test: build/tests/test_runner
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./build/tests/test_runner


build/tests/test_runner: $(OBJ_TEST) tetris.a
	$(CC) $(CFLAGS) $(OBJ_TEST) build/brick_game/tetris/tetris.a -o build/tests/test_runner -lcheck -lm -lsubunit

build/tests/%.o: src/tests/%.c
	mkdir -p build/tests/
	$(CC) $(CFLAGS) -c $< -o $@

gcov_report: build/tests/test_runner_gcov
	build/tests/test_runner_gcov
	mkdir -p src/gcov
	geninfo build/brick_game/tetris -o src/gcov/coverage.info
	genhtml src/gcov/coverage.info -o src/gcov
	rm -f build/brick_game/tetris/*.gcda build/brick_game/tetris/*.gcno


build/tests/test_runner_gcov: $(OBJ_TEST) tetris_gcov.a
	$(CC) $(CFLAGS) $(CFLAGS_TEST) $(OBJ_TEST) build/brick_game/tetris/tetris_gcov.a -o build/tests/test_runner_gcov -lgcov -lcheck -lm -lsubunit

tetris_gcov.a: $(OBJ_GCOV)
	ar rcs build/brick_game/tetris/tetris_gcov.a $(OBJ_GCOV)

build/brick_game/tetris/%_gcov.o: src/brick_game/tetris/%.c
	mkdir -p build/brick_game/tetris/
	$(CC) $(CFLAGS) $(CFLAGS_TEST) -c $< -o $@

# other
dist: clean
	tar -czvf brick_game.tar.gz src Makefile

clang:
	clang-format -n src/*/*.[ch] src/*/*/*.[ch]
	clang-format -i src/*/*.[ch] src/*/*/*.[ch]

clean:
	rm -rf build
	rm -rf src/dvi src/gcov brick_game.tar.gz
	rm -f src/refman.pdf
	rm -f *.gcno *.gcda
	rm -f brick_game.tar.gz

install: all
	cp build/tetris /usr/local/bin/
	touch /usr/local/bin/brick_game_high_score.txt

uninstall: clean
	rm /usr/local/bin/tetris
	rm /usr/local/bin/brick_game_high_score.txt

dvi: 
	mkdir -p src/dvi
	@cd src && doxygen Doxyfile  > /dev/null 2>&1 && cd ..
	@cd src/dvi && latexmk -silent -pdf refman.tex && cd ../..
	mv src/dvi/refman.pdf src/refman.pdf
	rm -rf src/dvi


# apt-get install texlive
# apt-get install texlive-latex-extra
# apt-get install doxygen
