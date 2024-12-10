#include <stdlib.h>

#include "tetris.h"
#include "../brick_game.h"

int** create_matrix(int str, int col) {
  int **field = calloc(str, sizeof(int *));
  if (field) {
    int err = 0;
    for (int i = 0; i < str && !err; i++) {
      field[i] = calloc(col, sizeof(int));
      if (!field[i]) {
        err = 1;
        i--;
        while (i >= 0) {
          free(field[i]);
          field[i] = NULL;
          i--;
        }
        free(field);
        field = NULL;
      }
    }
  }
  return field;
}

// typedef struct {
//   int **field;
//   int **next;
//   int score;
//   int high_score;
//   int level;
//   int speed;
//   int pause;
// } GameInfo_t;

int set_score() { return 0; }

int set_high_score() { return 0; }

int set_level() { return 0; }

int set_speed() { return 0; }

int set_pause() { return 0; }

GameInfo_t updateCurrentState() {
  GameInfo_t gameinfo = {0};
  gameinfo.field =  create_matrix(20,10);
  gameinfo.next = create_matrix(4,4);
  gameinfo.score = set_score();
  gameinfo.high_score = set_high_score();
  gameinfo.level = set_level();
  gameinfo.speed = set_speed();
  gameinfo.pause = set_pause();

  return gameinfo;
}
