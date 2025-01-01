#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h> 
#include "../brick_game.h"

#ifndef TETRIS_H_
#define TETRIS_H_

#define START 0
#define SPAWN 1
#define MOVING 2
#define SHIFTING 3
#define ATTACHING 4
#define PAUSE 5
#define GAME_OVER 6

#define FIELD_WIDTH 10
#define FIELD_HEIGHT 20
#define TETRAMINO_SIZE 5

typedef struct
{
    int **field_simple;
    int **tetramino_current;
    int x_coord;
    int y_coord;
} Field_data;

GameInfo_t* getInfo();
int* getState();
void create_next_tetramino(int ***tetramino);
void create_matrix(int ***matrix, int str, int col);
int set_score();
int set_high_score();
int set_level();
int set_speed();
int set_pause();
void move_left(bool hold, int state, GameInfo_t info);
void move_right(bool hold, int state, GameInfo_t info);
void move_down(bool hold, int state, GameInfo_t info);
void rotate(bool hold, int state, GameInfo_t info);



#endif