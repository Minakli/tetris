#ifndef TEST_H
#define TEST_H

#include <check.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "../brick_game/tetris/tetris.h"
#include "../gui/cli/main.h"

Suite *test_tetris(void);

Suite *test_matrix(void);

#endif
