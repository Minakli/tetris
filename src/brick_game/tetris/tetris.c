#include "./tetris.h"

GameInfo_t *getInfo() {
  static GameInfo_t info = {NULL, NULL, 0, 0, 1, 1, 0};

  if (!(info.field) && (info.pause != -1 && info.pause != -2)) {
    if (create_matrix(&(info.field), FIELD_HEIGHT, FIELD_WIDTH))
      info.pause = -1;
  }
  if (!(info.next) && (info.pause != -1 && info.pause != -2)) {
    if (create_matrix(&(info.next), TETRAMINO_SIZE, TETRAMINO_SIZE))
      info.pause = -1;
  }
  if (info.pause != -1 && info.pause != -2) {
    int tmp_high_score = 0;
    if (info.high_score < 1) {
      FILE *file = fopen("./brick_game_high_score.txt", "r");
      if (file) {
        fscanf(file, "%d", &tmp_high_score);
        if (info.high_score < tmp_high_score) info.high_score = tmp_high_score;
        fclose(file);
      }
    }
  }
  return &info;
}

Field_data *getData() {
  static Field_data data = {NULL, NULL, 0, 0};
  if (getInfo()->pause != -1 && getInfo()->pause != -2) {
    if (!data.field_simple) {
      if (create_matrix(&(data.field_simple), FIELD_HEIGHT, FIELD_WIDTH))
        getInfo()->pause = 1;
    }
    if (!data.tetramino_current) {
      if (create_matrix(&(data.tetramino_current), TETRAMINO_SIZE,
                        TETRAMINO_SIZE))
        getInfo()->pause = 1;
    }
  }
  return &data;
}

int *getState() {
  static int state = START;
  return &state;
}

GameInfo_t updateCurrentState() {
  if (getInfo()->pause != -1 && getInfo()->pause != -2) {
    getData();
    getState();
    switch (*getState()) {
      case MOVING:
        timer();
        break;
      case SHIFTING:
        move_down();
        break;
      case SPAWN:
        if (!spawn_next()) {
          *getState() = MOVING;
        } else {
          *getState() = GAME_OVER;
        }
        break;
      case ATTACHING:
        sum_matrix(&(getData()->field_simple));
        check_fill();
        *getState() = SPAWN;
        break;
      case GAME_OVER:
        saveResult();
        break;
      default:
        break;
    }
  } else {
    remove_matrix(&(getData()->tetramino_current), TETRAMINO_SIZE);
    remove_matrix(&(getData()->field_simple), FIELD_HEIGHT);
    remove_matrix(&(getInfo()->next), TETRAMINO_SIZE);
    remove_matrix(&(getInfo()->field), FIELD_HEIGHT);
  }
  return *(getInfo());
}

void userInput(UserAction_t action, bool hold) {
  switch (action) {
    case Start:
      if (*getState() == START) {
        create_next(&(getInfo()->next));
        *getState() = SPAWN;
      } else if (*getState() == PAUSE || *getState() == GAME_OVER) {
        saveResult();
        restartGame();
        create_next(&(getInfo()->next));
        *getState() = SPAWN;
      }
      break;
    case Pause:
      if (*getState() == PAUSE) {
        *getState() = MOVING;
        getInfo()->pause = 0;
      } else {
        *getState() = PAUSE;
        getInfo()->pause = 1;
      }
      break;
    case Terminate:
      saveResult();
      getInfo()->pause = -2;
      remove_matrix(&(getData()->tetramino_current), TETRAMINO_SIZE);
      remove_matrix(&(getData()->field_simple), FIELD_HEIGHT);
      remove_matrix(&(getInfo()->next), TETRAMINO_SIZE);
      remove_matrix(&(getInfo()->field), FIELD_HEIGHT);
      break;
    case Left:
      if (*getState() == MOVING) move_left();
      break;
    case Right:
      if (*getState() == MOVING) move_right();
      break;
    case Down:
      if (*getState() == MOVING) move_down();
      break;
    case Up:
      break;
    case Action:
      rotate();
      break;
    default:
      break;
  }
}

void restartGame() {
  for (int i = 0; i < FIELD_HEIGHT; i++) {
    for (int j = 0; j < FIELD_WIDTH; j++) {
      getData()->field_simple[i][j] = 0;
      getInfo()->field[i][j] = 0;
      if (i < TETRAMINO_SIZE && j < TETRAMINO_SIZE) {
        getData()->tetramino_current[i][j] = 0;
        getInfo()->next[i][j] = 0;
      }
    }
  }
  getInfo()->level = 0;
  getInfo()->score = 0;
  getInfo()->speed = 0;
  getInfo()->pause = 0;
}

int create_matrix(int ***matrix, int str, int col) {
  int err = 0;
  *matrix = calloc(str, sizeof(int *));
  if (*matrix) {
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
  } else {
    err = 1;
  }
  return err;
}

void remove_matrix(int ***matrix, int str) {
  if (*matrix) {
    for (int i = 0; i < str; i++) {
      if ((*matrix)[i]) {
        free((*matrix)[i]);
        (*matrix)[i] = NULL;
      }
    }
  }
  if (*matrix) {
    free(*matrix);
    *matrix = NULL;
  }
}

int spawn_next() {
  int err = 0;
  for (int i = 0; i < TETRAMINO_SIZE; i++) {
    for (int j = 0; j < TETRAMINO_SIZE; j++) {
      (getData()->tetramino_current)[i][j] = (getInfo()->next)[i][j];
    }
  }
  getData()->x_coord = 3;
  getData()->y_coord = -3;
  if (check_collision(getData()->tetramino_current, -2, 3)) {
    err = 1;
    getInfo()->pause = -10;
  } else {
    getData()->current_type = getData()->next_type;
    create_next(&(getInfo()->next));
  }
  return err;
}

int sum_matrix(int ***result_field) {
  int result = 0;
  int y_coord = getData()->y_coord;
  int x_coord = getData()->x_coord;
  for (int i = 0; i < FIELD_HEIGHT; i++) {
    for (int j = 0; j < FIELD_WIDTH; j++) {
      getInfo()->field[i][j] = getData()->field_simple[i][j];
    }
  }

  for (int i = y_coord; i < y_coord + TETRAMINO_SIZE && !result; i++) {
    for (int j = x_coord; j < x_coord + TETRAMINO_SIZE && !result; j++) {
      if (i >= 0 && j >= 0 && i < FIELD_HEIGHT && j < FIELD_WIDTH) {
        (*result_field)[i][j] =
            getData()->tetramino_current[i - y_coord][j - x_coord] ||
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
  getData()->next_type = "IOTLJSZ"[sign];
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

void move_left() {
  if (!check_collision(getData()->tetramino_current, getData()->y_coord,
                       getData()->x_coord - 1)) {
    getData()->x_coord--;
    sum_matrix(&(getInfo()->field));
  }
}

void move_right() {
  if (!check_collision(getData()->tetramino_current, getData()->y_coord,
                       getData()->x_coord + 1)) {
    getData()->x_coord++;
    sum_matrix(&(getInfo()->field));
  }
}

void move_down() {
  if (!check_collision(getData()->tetramino_current, getData()->y_coord + 1,
                       getData()->x_coord)) {
    getData()->y_coord++;
    sum_matrix(&(getInfo()->field));
    *getState() = MOVING;
  } else {
    *getState() = ATTACHING;
  }
}

void rotate_left() {
  if (!turn_tetramino('l', 0)) {
    turn_tetramino('L', 0);
  } else if (!turn_tetramino('l', 1)) {
    move_right();
    turn_tetramino('L', 0);
  } else if (!turn_tetramino('l', -1)) {
    move_left();
    turn_tetramino('L', 0);
  }
}

void rotate() {
  if (getData()->current_type == 'T' || getData()->current_type == 'L' ||
      getData()->current_type == 'J') {
    rotate_left();
  } else if ((getData()->current_type == 'S' &&
              getData()->tetramino_current[1][3] == 1) ||
             getData()->current_type == 'Z' &&
                 getData()->tetramino_current[1][1] == 1) {
    rotate_left();
  } else if ((getData()->current_type == 'S' &&
              getData()->tetramino_current[1][3] == 0) ||
             (getData()->current_type == 'Z' &&
              getData()->tetramino_current[1][1] == 0)) {
    if (!turn_tetramino('r', 0)) {
      turn_tetramino('R', 0);
    } else if (!turn_tetramino('r', 1)) {
      move_right();
      turn_tetramino('R', 0);
    } else if (!turn_tetramino('r', -1)) {
      move_left();
      turn_tetramino('R', 0);
    }
  } else if (getData()->current_type == 'I') {
    if (getData()->tetramino_current[2][0] == 1) {
      if (!turn_tetramino('r', 0)) turn_tetramino('R', 0);
    } else {
      if (!turn_tetramino('l', 0)) {
        turn_tetramino('L', 0);
      } else if (!turn_tetramino('l', 1)) {
        move_right();
        turn_tetramino('L', 0);
      } else if (!turn_tetramino('l', 2)) {
        move_right();
        move_right();
        turn_tetramino('L', 0);
      } else if (!turn_tetramino('l', -1)) {
        move_left();
        turn_tetramino('L', 0);
      }
    }
  }
}

int turn_tetramino(char direction, int x_pos) {
  int cant_turn = 0;
  int tmp_matrix[TETRAMINO_SIZE][TETRAMINO_SIZE];
  for (int i = 0; i < TETRAMINO_SIZE; i++) {
    for (int j = 0; j < TETRAMINO_SIZE; j++) {
      if (direction == 'L' || direction == 'l') {
        tmp_matrix[i][j] =
            getData()->tetramino_current[TETRAMINO_SIZE - 1 - j][i];
      } else if (direction == 'R' || direction == 'r') {
        tmp_matrix[i][j] =
            getData()->tetramino_current[j][TETRAMINO_SIZE - 1 - i];
      }
    }
  }
  for (int i = 0; i < TETRAMINO_SIZE && !cant_turn; i++) {
    for (int j = 0; j < TETRAMINO_SIZE && !cant_turn; j++) {
      if (direction == 'l' || direction == 'r') {
        int y = getData()->y_coord + i;
        int x = getData()->x_coord + j + x_pos;
        if (y >= 0 && y < 20 && x >= 0 && x < 10) {
          if (tmp_matrix[i][j] + getData()->field_simple[y][x] > 1) cant_turn++;
        } else if (y > 19 || x < 0 || x > 9) {
          if (tmp_matrix[i][j] > 0) cant_turn++;
        }

      } else if (direction == 'L' || direction == 'R') {
        getData()->tetramino_current[i][j] = tmp_matrix[i][j];
      }
    }
  }
  sum_matrix(&(getInfo()->field));
  return cant_turn;
}

int check_collision(int **tetramino, int y_coord, int x_coord) {
  int lock = 0;
  for (int i = 0; i < TETRAMINO_SIZE && !lock; i++) {
    for (int j = 0; j < TETRAMINO_SIZE && !lock; j++) {
      int y = y_coord + i;
      int x = x_coord + j;
      if (y >= 0 && y < 20 && x >= 0 && x < 10) {
        if (tetramino[i][j] + getData()->field_simple[y][x] > 1) lock++;
      } else if (y > 19 || x < 0 || x > 9) {
        if (tetramino[i][j] > 0) lock++;
      }
    }
  }
  return lock;
}

void timer() {
  static struct timeval start = {0};
  if (start.tv_sec == 0) gettimeofday(&start, NULL);
  struct timeval end = {0};
  gettimeofday(&end, NULL);
  if ((end.tv_sec * 1000 + end.tv_usec / 1000) -
          (start.tv_sec * 1000 + start.tv_usec / 1000) >
      650 - 50 * getInfo()->level) {
    *getState() = SHIFTING;
    start = end;
  }
  getInfo()->speed = 650 - 50 * getInfo()->level;
}

void check_fill() {
  int full_lines_count = 0;
  for (int i = 0; i < FIELD_HEIGHT; i++) {
    int is_full = 1;
    for (int j = 0; j < FIELD_WIDTH && is_full; j++) {
      if (getData()->field_simple[i][j] == 0) is_full = 0;
    }
    if (is_full) {
      delete_full_line(i);
      full_lines_count++;
    }
  }
  sum_matrix(&(getInfo()->field));
  int score = 1;
  while (full_lines_count--) score *= 2;
  getInfo()->score += 100 * (score - 1);
  getInfo()->level = getInfo()->score / 600 + 1;
  if (getInfo()->level > 10) getInfo()->level = 10;
}

void delete_full_line(int y_index) {
  for (int i = y_index; i > 0; i--) {
    for (int j = 0; j < FIELD_WIDTH; j++) {
      getData()->field_simple[i][j] = getData()->field_simple[i - 1][j];
    }
  }
  for (int j = 0; j < FIELD_WIDTH; j++) {
    getData()->field_simple[0][j] = 0;
  }
}

void saveResult() {
  FILE *file = fopen("./brick_game_high_score.txt", "w");
  if (file) {
    if (getInfo()->score > getInfo()->high_score) {
      fprintf(file, "%d", getInfo()->score);
    }
    fclose(file);
  }
}