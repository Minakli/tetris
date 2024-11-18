CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11

SRC_UI = $(shell find ./src/gui -name "*.c")
SRC_GAME = $(shell find ./src/brick_game -name "*.c")
BUILD_DIR = build
OBJ_GAME = $(patsubst %.c $(BUILD_DIR)/%.o $(SRC))

play: 
	/usr/local/bin/brick_game

install: brick_game
	cp brick game /usr/local/bin/

# $(CC) $(CFLAGS) -c $(SRC)
	$(CC) -c $(SRC)
game: $(OBJ)




# test: clean test

unintall:
	rm -f /usr/local/bin/brick_game
	rm -f bin/*.*
