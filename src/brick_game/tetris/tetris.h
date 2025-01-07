#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h> 
#include <sys/time.h>

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
    int next_type;
    int current_type;
} Field_data;

GameInfo_t* getInfo();
Field_data *getData();
int spawn_next();
int* getState();
void create_next(int ***tetramino);
int create_matrix(int ***matrix, int str, int col);
void clear_matrix(int ***matrix, int row, int col);
int check_collision(int **tetramino, int y_coord, int x_coord);
void remove_matrix(int ***matrix, int str);
int sum_matrix(int ***result_field);
void move_left();
void move_right();
void move_down();
void rotate();
void rotate_left();
void timer();
int turn_tetramino(char direction, int x_pos);
void check_fill();
void delete_full_line(int y_index);
void restartGame();
void saveResult();

#endif