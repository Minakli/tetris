#ifndef MAIN_H_
#define MAIN_H_

/**
 * @file main.h
 * @brief Header file for the main game loop and related operations.
 *
 * This file contains function declarations and constants related to
 * the main game loop, printing the game field, and managing game states.
 */

#include <stdio.h>
#include <ncurses.h>
#include <unistd.h>
#include "../../brick_game/brick_game.h"

#define MEM_ERROR -1
#define EXIT -2
#define GAME_OVER 6

int printGame(GameInfo_t info);
void printField(int **field);
void printNext(int **next) ;

#endif