#include "test.h"

START_TEST(test_tetris_1) {
  GameInfo_t *info = getInfo();

  ck_assert_ptr_nonnull(info);
  ck_assert_ptr_nonnull(info->field);
  ck_assert_ptr_nonnull(info->next);
  info->field[1][1] = 1;
  info->next[1][1] = 1;
  ck_assert_int_eq(info->score, 0);
  ck_assert_int_eq(info->high_score, 0);
  ck_assert_int_eq(info->level, 1);
  ck_assert_int_eq(info->speed, 1);
  ck_assert_int_eq(info->pause, 0);
}
END_TEST

START_TEST(test_tetris_2) {
  Field_data *data = getData();

  ck_assert_ptr_nonnull(data);
  ck_assert_ptr_nonnull(data->field_simple);
  ck_assert_ptr_nonnull(data->tetramino_current);
  ck_assert_int_eq(data->current_type, 0);
  ck_assert_int_eq(data->next_type, 0);
  ck_assert_int_eq(data->x_coord, 0);
  ck_assert_int_eq(data->y_coord, 0);
}
END_TEST

START_TEST(test_tetris_3) {
  *getState() = SPAWN;
  ck_assert_int_eq(getData()->y_coord, -0);
  updateCurrentState();
  ck_assert_int_eq(*getState(), MOVING);
  updateCurrentState();
  ck_assert_int_eq(getData()->y_coord, -3);
  *getState() = ATTACHING;
  ck_assert_int_eq(*getState(), ATTACHING);
  updateCurrentState();
  ck_assert_int_eq(*getState(), SPAWN);
  updateCurrentState();
  ck_assert_int_eq(*getState(), MOVING);

  getInfo()->score = 1000;
  *getState() = GAME_OVER;
  updateCurrentState();

  FILE *file = fopen("build/brick_game_high_score.txt", "r");
  int tmp_high_score = 0;
  if (file) {
    fscanf(file, "%d", &tmp_high_score);
    fclose(file);
  }
  ck_assert_int_eq(tmp_high_score, 1000);

  getInfo()->pause = MEM_ERROR;
  updateCurrentState();
  ck_assert_ptr_null(getData()->tetramino_current);
  ck_assert_ptr_null(getData()->field_simple);
  ck_assert_ptr_null(getInfo()->next);
  ck_assert_ptr_null(getInfo()->field);

  getInfo()->pause = 0;
  updateCurrentState();
  ck_assert_ptr_nonnull(getData()->tetramino_current);
  ck_assert_ptr_nonnull(getData()->field_simple);
  ck_assert_ptr_nonnull(getInfo()->next);
  ck_assert_ptr_nonnull(getInfo()->field);
}
END_TEST

START_TEST(test_tetris_4) {
  getInfo();
  getData();
  *getState() = START;
  updateCurrentState();
  userInput(Start, false);
  ck_assert_int_eq(*getState(), SPAWN);

  *getState() = MOVING;
  userInput(Pause, false);
  ck_assert_int_eq(*getState(), PAUSE);
  userInput(Pause, false);
  ck_assert_int_eq(*getState(), MOVING);
  ck_assert_int_eq(getData()->x_coord, 3);
  ck_assert_int_eq(getData()->y_coord, -3);
  ck_assert_int_eq(*getState(), MOVING);
  userInput(Left, false);
  ck_assert_int_eq(getData()->x_coord, 2);
  ck_assert_int_eq(*getState(), MOVING);
  userInput(Right, false);
  ck_assert_int_eq(getData()->x_coord, 3);
  ck_assert_int_eq(*getState(), MOVING);
  userInput(Down, false);
  ck_assert_int_eq(getData()->y_coord, -2);
  ck_assert_int_eq(*getState(), MOVING);
  userInput(Action, false);
  ck_assert_int_eq(*getState(), MOVING);
}
END_TEST

START_TEST(test_tetris_5) {
  GameInfo_t *info = getInfo();
  ck_assert_ptr_nonnull(info);
  ck_assert_ptr_nonnull(info->field);
  ck_assert_ptr_nonnull(info->next);
  info->level = 3;
  info->speed = 3;
  info->score = 3;
  info->pause = 3;
  ck_assert_int_eq(info->level, 3);
  ck_assert_int_eq(info->score, 3);
  ck_assert_int_eq(info->speed, 3);
  ck_assert_int_eq(info->pause, 3);

  restartGame();

  ck_assert_int_eq(info->field[1][1], 0);
  ck_assert_int_eq(info->next[1][1], 0);
  ck_assert_int_eq(info->level, 0);
  ck_assert_int_eq(info->score, 0);
  ck_assert_int_eq(info->speed, 0);
  ck_assert_int_eq(info->pause, 0);
}
END_TEST

START_TEST(test_tetris_6) {
  getData()->current_type = 'T';
  getData()->tetramino_current[0][0] = 1;
  ck_assert_int_eq(getData()->tetramino_current[0][0], 1);
  rotate();
  ck_assert_int_eq(getData()->tetramino_current[0][0], 0);
  ck_assert_int_eq(getData()->tetramino_current[0][4], 1);
  getData()->current_type = 'S';
  ck_assert_int_eq(getData()->tetramino_current[0][0], 0);
  rotate();
  ck_assert_int_eq(getData()->tetramino_current[0][0], 1);
  getData()->current_type = 'S';
  rotate();
  ck_assert_int_eq(getData()->tetramino_current[4][0], 1);
}

START_TEST(test_tetris_7) {
  for (int i = 0; i < 10; i++) {
    createNext(&(getInfo()->next));
    sleep(1);
  }
  clearMatrix(&(getData()->tetramino_current), TETRAMINO_SIZE, TETRAMINO_SIZE);
  getData()->current_type = 'I';
  for (int i = 0; i < TETRAMINO_SIZE; i++) {
    getData()->tetramino_current[2][i] = 1;
  }
  rotate();
  for (int i = 0; i < TETRAMINO_SIZE; i++) {
    ck_assert_int_eq(getData()->tetramino_current[i][2], 1);
  }
  getData()->x_coord = -2;
  rotate();
  for (int i = 0; i < TETRAMINO_SIZE; i++) {
    ck_assert_int_eq(getData()->tetramino_current[2][i], 1);
  }
}
END_TEST

START_TEST(test_tetris_8) {
  for (int i = 0; i < FIELD_WIDTH; i++) {
    getData()->field_simple[5][i] = 1;
  }
  for (int i = 0; i < FIELD_WIDTH; i++) {
    ck_assert_int_eq(getData()->field_simple[5][i], 1);
  }
  deleteFullLine(5);
  for (int i = 0; i < FIELD_WIDTH; i++) {
    ck_assert_int_eq(getData()->field_simple[5][i], 0);
  }

  userInput(Terminate, false);
  ck_assert_ptr_null(getData()->tetramino_current);
  ck_assert_ptr_null(getData()->field_simple);
  ck_assert_ptr_null(getInfo()->next);
  ck_assert_ptr_null(getInfo()->field);
}
END_TEST

Suite *test_tetris(void) {
  Suite *s = suite_create("Tetris");
  TCase *tc_core = tcase_create("Core");
  tcase_add_test(tc_core, test_tetris_1);
  tcase_add_test(tc_core, test_tetris_2);
  tcase_add_test(tc_core, test_tetris_3);
  tcase_add_test(tc_core, test_tetris_4);
  tcase_add_test(tc_core, test_tetris_5);
  tcase_add_test(tc_core, test_tetris_6);
  tcase_add_test(tc_core, test_tetris_7);
  tcase_add_test(tc_core, test_tetris_8);

  suite_add_tcase(s, tc_core);
  return s;
}
