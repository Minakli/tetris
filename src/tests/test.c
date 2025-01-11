#include "test.h"

int main(void) {
  int failed = 0;
  int success = 0;
  Suite *brick_game_test[] = {
      test_tetris(),
      // test_matrix(),
  };

  for (unsigned long i = 0;
       i < sizeof(brick_game_test) / sizeof(brick_game_test[0]); i++) {
    SRunner *sr = srunner_create(brick_game_test[i]);
    srunner_set_fork_status(sr, CK_NOFORK);
    srunner_run_all(sr, CK_NORMAL);
    failed += srunner_ntests_failed(sr);
    success += srunner_ntests_run(sr);
    srunner_free(sr);
  }
  printf("\033[31m========= FAILED : %-3d =========\033[0m\n", failed);
  printf("\033[32m========= SUCCESS : %-3d =========\033[0m\n", success);
  return (failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
