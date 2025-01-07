#ifndef MAIN_H_
#define MAIN_H_

#include <stdio.h>
#include <ncurses.h>
#include <unistd.h>
#include "../../brick_game/brick_game.h"

int print_game(GameInfo_t info);
void print_field(int **field);
void print_next(int **next) ;

#endif