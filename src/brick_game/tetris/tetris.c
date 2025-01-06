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
  set_score();
  // set_high_score();
  // set_level();
  // set_speed();
  game_info.speed = 0;
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
  getInfo()->high_score++;
  switch (*getState()) {
    case MOVING:
      timer();
      break;
    case SHIFTING:
      move_down();
      break;
    case PAUSE:
      /* code */
      break;
    case SPAWN:
      // check_fill();
      spawn_next();
      *getState() = MOVING;
      break;
    case ATTACHING:
      sum_matrix(&(getData()->field_simple));
      check_fill();
      // sum_matrix(&(getInfo()->field_simple));
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
      // if(*getState() == MOVING)
      getInfo()->high_score = action;
      move_left();
      break;
    case Right:
      getInfo()->high_score = action;

      if (*getState() == MOVING) move_right();
      break;
    case Down:
      getInfo()->high_score = action;
      move_down();
      break;
    case Up:
      getData()->y_coord--;
      sum_matrix(&(getInfo()->field));
      break;
    case Action:
      rotate();
      // rotate();
      break;
    default:
      break;
  }
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
  getData()->current_type = getData()->next_type;
  create_next(&(getInfo()->next));
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

int set_score() {
  // if(getData()->x_coord) getInfo()->score = getData()->x_coord;
}
int set_high_score() {}
int set_level() { getInfo()->level = *getState(); }
void check_attaching() {}
int set_speed() {}
int set_pause() {}

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
void rotate() {
  if (getData()->current_type == 'T' || getData()->current_type == 'L' ||
      getData()->current_type == 'J') {
    if (!turn_tetramino('l', 0)) {
      turn_tetramino('L', 0);
    } else if (!turn_tetramino('l', 1)) {
      move_right();
      turn_tetramino('L', 0);
    } else if (!turn_tetramino('l', -1)) {
      move_left();
      turn_tetramino('L', 0);
    }
  } else if ((getData()->current_type == 'S' &&
              getData()->tetramino_current[1][3] == 1) ||
             getData()->current_type == 'Z' &&
                 getData()->tetramino_current[1][1] == 1) {
    if (!turn_tetramino('l', 0)) {
      turn_tetramino('L', 0);
    } else if (!turn_tetramino('l', 1)) {
      move_right();
      turn_tetramino('L', 0);
    } else if (!turn_tetramino('l', -1)) {
      move_left();
      turn_tetramino('L', 0);
    }
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
  if ((end.tv_sec * 100 + end.tv_usec / 10000) -
          (start.tv_sec * 100 + start.tv_usec / 10000) >
      75 - 5 * getInfo()->speed) {
    *getState() = SHIFTING;
    start = end;
  }
}

int check_fill() {
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
  return full_lines_count;
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