/**
 * @file tetris.h
 * @brief Header file for the Tetris game logic.
 * 
 * This file contains definitions, constants, and function prototypes
 * required for the Tetris game implementation.
 */
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h> 
#include <sys/time.h>

#include "../brick_game.h"

#ifndef TETRIS_H_
#define TETRIS_H_

/** @defgroup GameStates Game States
 *  @brief Constants representing various game states.
 *  @{
 */
#define START 0
#define SPAWN 1
#define MOVING 2
#define SHIFTING 3
#define ATTACHING 4
#define PAUSE 5
#define GAME_OVER 6

/** @defgroup ErrorCodes Error Codes
 *  @brief Constants for error codes.
 *  @{
 */
#define MEM_ERROR -1
#define EXIT -2

/** @defgroup GameDimensions Game Dimensions
 *  @brief Constants for field dimensions and tetramino size.
 *  @{
 */
#define FIELD_WIDTH 10
#define FIELD_HEIGHT 20
#define TETRAMINO_SIZE 5

/**
 * @struct Field_data
 * @brief Structure representing the game field and tetramino state.
 */
typedef struct
{
    int **field_simple;
    int **tetramino_current;
    int x_coord;
    int y_coord;
    int next_type;
    int current_type;
} Field_data;

GameInfo_t* getInfo();
Field_data *getData();
int spawn_next();
int* getState();
void createNext(int ***tetramino);
int createMatrix(int ***matrix, int str, int col);
void clearMatrix(int ***matrix, int row, int col);
int checkCollision(int **tetramino, int y_coord, int x_coord);
void removeMatrix(int ***matrix, int str);
int sumMatrix(int ***result_field);
void moveLeft();
void moveRight();
void moveDown();
void rotate();
void rotateLeft();
void timer();
int turnTetramino(char direction, int x_pos);
void checkFill();
void deleteFullLine(int y_index);
void restartGame();
void saveResult();

#endif