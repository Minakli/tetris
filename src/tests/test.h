#ifndef TEST_H
#define TEST_H

#include <check.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include "../gui/cli/main.h"
#include "../brick_game/tetris/tetris.h"

Suite *test_gui(void);

Suite *test_tetris(void);

// Suite *tests_functions(void);

// Suite *tests_mul(void);

// Suite *tests_other(void);

// Suite *tests_add(void);

// Suite *tests_comparison(void);

#endif
