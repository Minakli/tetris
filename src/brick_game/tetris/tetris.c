#include "./tetris.h"

GameInfo_t *getInfo() {
  static GameInfo_t game_info = {NULL, NULL, 0, -1, 1, 1, 0};

  if (!(game_info.field))
    create_matrix(&(game_info.field), FIELD_HEIGHT, FIELD_WIDTH);
  if (!(game_info.next))
    create_matrix(&(game_info.next), TETRAMINO_SIZE, TETRAMINO_SIZE);
  if (game_info.high_score < 0) {
    game_info.high_score = 0;
    FILE *file = fopen("./brick_game_hight_score.txt", "r");
    if (file) {
      fscanf(file, "%d", &(game_info.high_score));
      fclose(file);
    }
  }
  // set_score();
  // set_high_score();
  // set_level();
  // set_speed();
  // set_pause();
  game_info.level = *getState();
  return &game_info;
}

Field_data *getData() {
  static Field_data data = {NULL, NULL, 0, 0};
  if (!data.field_simple)
    create_matrix(&(data.field_simple), FIELD_HEIGHT, FIELD_WIDTH);
  if (!(data.tetramino_current))
    create_matrix(&(data.tetramino_current), TETRAMINO_SIZE, TETRAMINO_SIZE);
  return &data;
}

int *getState() {
  static int state = START;
  return &state;
}

GameInfo_t updateCurrentState() {
  getInfo();
  getData();
  getState();
  static clock_t current = -1;
  if (current == -1) current = clock();

  switch (*getState()) {
    case MOVING:
      clock_t next = clock();
      if ((((double)(next - current)) / CLOCKS_PER_SEC) >
          0.75 - 0.05 * getInfo()->speed) {
        current = next;
        *getState() = SHIFTING;
      }
      break;
    case SHIFTING:
      move_down();
      break;
    case PAUSE:
      /* code */
      break;
    case SPAWN:
      spawn_next();
      *getState() = MOVING;
      break;
    case ATTACHING:
      sum_matrix(&(getData()->field_simple));
      *getState() = SPAWN;
      break;
    case GAME_OVER:
      /* code */
      break;
    default:
      break;
  }
  // sum_matrix(&(getInfo()->field));
  return *(getInfo());
}

void userInput(UserAction_t action, bool hold) {
  switch (action) {
    case Start:
      if (*getState() == START) {
        create_next(&(getInfo()->next));
        *getState() = SPAWN;
      }
      break;
    case Pause:
      if (*getState() == MOVING) *getState() = PAUSE;
      if (*getState() == PAUSE) *getState() = MOVING;
      break;
    case Terminate:
      *getState() = GAME_OVER;
      break;
    case Left:
      if(*getState() == MOVING) move_left();
      break;
    case Right:
      if(*getState() == MOVING) move_right();
      break;
    case Down:
      if(*getState() == MOVING) move_down();
      break;
    case Action:
      rotate(hold, *getState(), *getInfo());
      break;
    default:
      break;
  }
}

void create_matrix(int ***matrix, int str, int col) {
  *matrix = calloc(str, sizeof(int *));
  if (*matrix) {
    int err = 0;
    for (int i = 0; i < str && !err; i++) {
      (*matrix)[i] = calloc(col, sizeof(int));
      if (!(*matrix)[i]) {
        err = 1;
        i--;
        while (i >= 0) {
          free((*matrix)[i]);
          (*matrix)[i] = NULL;
          i--;
        }
        free((*matrix));
        *matrix = NULL;
      }
    }
  }
}

void remove_matrix(int ***matrix, int str) {
  for (int i = 0; i < str; i++) {
    if ((*matrix)[i]) {
      free((*matrix)[i]);
      (*matrix)[i] = NULL;
    }
  }
  if (*matrix) {
    free(*matrix);
    *matrix = NULL;
  }
}

void spawn_next() {
  for (int i = 0; i < TETRAMINO_SIZE; i++) {
    for (int j = 0; j < TETRAMINO_SIZE; j++) {
      (getData()->tetramino_current)[i][j] = (getInfo()->next)[i][j];
    }
  }
  getData()->x_coord = 3;
  getData()->y_coord = -3;
  create_next(&(getInfo()->next));
}

int sum_matrix(int ***result_field) {
  int result = 0;
  int y_coord = getData()->y_coord;
  int x_coord = getData()->x_coord;
  for (int i = y_coord; i < y_coord + TETRAMINO_SIZE && !result; i++) {
    for (int j = x_coord; j < x_coord + TETRAMINO_SIZE && !result; j++) {
      if (i >= 0 && j >= 0 && i < FIELD_HEIGHT && j < FIELD_WIDTH) {
        (*result_field)[i][j] =
            getData()->tetramino_current[i - y_coord][j - x_coord] +
            getData()->field_simple[i][j];
      }
    }
  }
  return result;
}

void create_next(int ***tetramino) {
  clear_matrix(tetramino, TETRAMINO_SIZE, TETRAMINO_SIZE);
  srand(time(NULL));
  static int prev_sign = -1;
  int sign = 0;
  do {
    sign = rand() % 7;
  } while (sign == prev_sign);
  prev_sign = sign;
  switch ("IOTLJSZ"[sign]) {
    case 'I':
      for (int i = 0; i < 4; i++) (*tetramino)[2][i] = 1;
      break;
    case 'O':
      (*tetramino)[1][1] = 1;
      (*tetramino)[1][2] = 1;
      (*tetramino)[2][1] = 1;
      (*tetramino)[2][2] = 1;
      break;
    case 'T':
      (*tetramino)[1][2] = 1;
      for (int i = 1; i < 4; i++) (*tetramino)[2][i] = 1;
      break;
    case 'L':
      (*tetramino)[1][3] = 1;
      for (int i = 1; i < 4; i++) (*tetramino)[2][i] = 1;
      break;
    case 'J':
      (*tetramino)[1][1] = 1;
      for (int i = 1; i < 4; i++) (*tetramino)[2][i] = 1;
      break;
    case 'S':
      (*tetramino)[1][2] = 1;
      (*tetramino)[1][3] = 1;
      (*tetramino)[2][1] = 1;
      (*tetramino)[2][2] = 1;
      break;
    case 'Z':
      (*tetramino)[1][1] = 1;
      (*tetramino)[1][2] = 1;
      (*tetramino)[2][2] = 1;
      (*tetramino)[2][3] = 1;
      break;
    default:
      break;
  }
}

void clear_matrix(int ***matrix, int row, int col) {
  for (int i = 0; i < row; i++) {
    for (int j = 0; j < col; j++) {
      (*matrix)[i][j] = 0;
    }
  }
}

int set_score() {}
int set_high_score() {}
int set_level() { getInfo()->level = *getState(); }
void check_attaching() {}
int set_speed() {}
int set_pause() {}
int check_collision_down() {
  int collision = 0;
  int y_coord = getData()->y_coord;
  int x_coord = getData()->x_coord;
  for (int t = x_coord; t < TETRAMINO_SIZE; t++) {
    if (FIELD_HEIGHT - y_coord >= 0 && FIELD_HEIGHT - y_coord < 5) {
      if (getData()->tetramino_current[FIELD_HEIGHT - y_coord - 1][t] == 1)
        collision = 1;
    }
  }
  y_coord++;
  for (int i = y_coord; i < y_coord + TETRAMINO_SIZE && !collision; i++) {
    for (int j = x_coord; j < x_coord + TETRAMINO_SIZE && !collision; j++) {
      if (i >= 0 && j >= 0 && i < FIELD_HEIGHT && j < FIELD_WIDTH) {
        if (getData()->tetramino_current[i - y_coord][j - x_coord] +
                getData()->field_simple[i][j] >
            1) {
          collision = 1;
        }
      }
    }
  }
  return collision;
}
void move_left() {
  getData()->x_coord--;
  sum_matrix(&(getInfo()->field));
}
void move_right() {
  getData()->x_coord++;
  sum_matrix(&(getInfo()->field));
}
void move_down() {
  if(!check_collision_down()) {
    getData()->y_coord++;
    sum_matrix(&(getInfo()->field));
    *getState() = MOVING;
  } else {
    *getState() = ATTACHING;
  }
}
void rotate(bool hold, int state, GameInfo_t info) {}