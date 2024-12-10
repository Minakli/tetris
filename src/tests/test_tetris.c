#include "test.h"

START_TEST(test_tet_1) {
//   s21_decimal a = {{3, 0, 0, 0}};
//   s21_decimal b = {{2, 0, 0, 0}};
//   s21_decimal c = {{0, 0, 0, 0}};
//   int error = s21_add(a, b, &c);
//   s21_decimal expected = {{5, 0, 0, 0}};
//   for (int i = 0; i < 4; i++) {
//     ck_assert_int_eq(c.bits[i], expected.bits[i]);
//   }
//   ck_assert_int_eq(error, 0);
}
END_TEST


Suite *test_tetris(void) {
  Suite *s = suite_create("Gui");
  TCase *tc_core = tcase_create("Core");
  tcase_add_test(tc_core, test_tet_1);
//   tcase_add_test(tc_core, s21_sub_2);


  suite_add_tcase(s, tc_core);
  return s;
}
