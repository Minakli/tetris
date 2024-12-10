#include <stdlib.h>

#include "tetris.h"

int **create_field() {
  int **field = calloc(20, sizeof(int *));
  if (field) {
    int err = 0;
    for (int i = 0; i < 20 && !err; i++) {
      field[i] = calloc(10, sizeof(int));
      if (!field[i]) {
        err = i;
        i--;
        while (i >= 0) {
          free(field[i]);
          field[i] = NULL;
        }
        free(field);
        field = NULL;
      }
    }
  }
  return field;
}

int **create_next_tetratino() {
  int **next = calloc(4, sizeof(int *));
  if (next) {
    int err = 0;
    for (int i = 0; i < 4 && !err; i++) {
      next[i] = calloc(4, sizeof(int));
      if (!next[i]) {
        err = i;
        i--;
        while (i >= 0) {
          free(next[i]);
          next[i] = NULL;
        }
        free(next);
        next = NULL;
      }
    }
  }
  return 0;
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

int **set_next_tetratino() { return NULL; }

int set_score() { return 0; }

int set_high_score() { return 0; }

int set_level() { return 0; }

int set_speed() { return 0; }

int set_pause() { return 0; }

GameInfo_t updateCurrentState() {
  GameInfo_t gameinfo = {0};
  gameinfo.field =  create_field();
  gameinfo.next = set_next_tetratino();
  gameinfo.score = set_score();
  gameinfo.high_score = set_high_score();
  gameinfo.level = set_level();
  gameinfo.speed = set_speed();
  gameinfo.pause = set_pause();

  return gameinfo;
}
