#include "./tetris.h"

/**
 * @brief Restart the game by resetting all the game data.
 *
 * Clears the game field, resets current and next tetromino data,
 * and resets the level, score, speed, and pause status.
 */
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

/**
 * @brief Creates a matrix of given size.
 *
 * Allocates memory for a matrix of size (str x col).
 * If allocation fails, the matrix is freed and set to NULL.
 *
 * @param matrix Pointer to a pointer to the matrix to be created.
 * @param str Number of rows in the matrix.
 * @param col Number of columns in the matrix.
 * @return int 0 if the matrix was successfully created, 1 if there was an
 * error.
 */
int createMatrix(int ***matrix, int str, int col) {
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

/**
 * @brief Frees the memory of a previously created matrix.
 *
 * This function will free each row of the matrix and then free the matrix
 * itself.
 *
 * @param matrix Pointer to the matrix to be freed.
 * @param str Number of rows in the matrix.
 */
void removeMatrix(int ***matrix, int str) {
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

/**
 * @brief Spawns the next tetromino in the game.
 *
 * Copies the next tetromino data into the current tetromino matrix,
 * sets the initial coordinates of the tetromino, and checks for collision.
 * If a collision is detected, the game ends.
 *
 * @return int 0 if the spawn was successful, 1 if there was a collision.
 */
int spawn_next() {
  int err = 0;
  for (int i = 0; i < TETRAMINO_SIZE; i++) {
    for (int j = 0; j < TETRAMINO_SIZE; j++) {
      (getData()->tetramino_current)[i][j] = (getInfo()->next)[i][j];
    }
  }
  getData()->x_coord = 3;
  getData()->y_coord = -3;
  if (checkCollision(getData()->tetramino_current, -2, 3)) {
    err = 1;
    getInfo()->pause = GAME_OVER;
  } else {
    getData()->current_type = getData()->next_type;
    createNext(&(getInfo()->next));
  }
  return err;
}

/**
 * @brief Summarizes the current state of the game field and tetromino.
 *
 * Combines the current tetromino and the game field into a result field,
 * checking for collisions and filling the field accordingly.
 *
 * @param result_field Pointer to the result field that will be updated.
 * @return int Returns 0 if the operation was successful, otherwise returns an
 * error code.
 */
int sumMatrix(int ***result_field) {
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

/**
 * @brief Creates a random next tetromino.
 *
 * Randomly generates a new tetromino type and fills the next tetromino matrix
 * with its shape.
 */
void createNext(int ***tetramino) {
  clearMatrix(tetramino, TETRAMINO_SIZE, TETRAMINO_SIZE);
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
      for (int i = 1; i < 3; i++) (*tetramino)[1][i] = 1;
      for (int i = 1; i < 3; i++) (*tetramino)[2][i] = 1;
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
      for (int i = 2; i < 4; i++) (*tetramino)[1][i] = 1;
      for (int i = 1; i < 3; i++) (*tetramino)[2][i] = 1;
      break;
    case 'Z':
      for (int i = 1; i < 3; i++) (*tetramino)[1][i] = 1;
      for (int i = 2; i < 4; i++) (*tetramino)[2][i] = 1;
      break;
    default:
      break;
  }
}

/**
 * @brief Clears a matrix by setting all its elements to zero.
 *
 * @param matrix Pointer to the matrix to be cleared.
 * @param row Number of rows in the matrix.
 * @param col Number of columns in the matrix.
 */
void clearMatrix(int ***matrix, int row, int col) {
  for (int i = 0; i < row; i++) {
    for (int j = 0; j < col; j++) {
      (*matrix)[i][j] = 0;
    }
  }
}

/**
 * @brief Moves the current tetromino one unit to the left.
 *
 * Checks if the tetromino can be moved left (without collision) and updates the
 * position.
 */
void moveLeft() {
  if (!checkCollision(getData()->tetramino_current, getData()->y_coord,
                      getData()->x_coord - 1)) {
    getData()->x_coord--;
    sumMatrix(&(getInfo()->field));
  }
}

/**
 * @brief Moves the current tetromino one unit to the right.
 *
 * Checks if the tetromino can be moved right (without collision) and updates
 * the position.
 */
void moveRight() {
  if (!checkCollision(getData()->tetramino_current, getData()->y_coord,
                      getData()->x_coord + 1)) {
    getData()->x_coord++;
    sumMatrix(&(getInfo()->field));
  }
}

/**
 * @brief Moves the current tetromino one unit down.
 *
 * Checks if the tetromino can be moved down (without collision). If it can, the
 * position is updated. If a collision occurs, the tetromino is fixed in place.
 */
void moveDown() {
  if (!checkCollision(getData()->tetramino_current, getData()->y_coord + 1,
                      getData()->x_coord)) {
    getData()->y_coord++;
    sumMatrix(&(getInfo()->field));
    *getState() = MOVING;
  } else {
    *getState() = ATTACHING;
  }
}

/**
 * @brief Rotates the current tetromino to the left.
 *
 * This function attempts to rotate the current tetromino in the
 * counterclockwise direction (left). If the rotation is not possible, it tries
 * to adjust the position by moving the tetromino and performing the rotation
 * again.
 */
void rotateLeft() {
  if (!turnTetramino('l', 0)) {
    turnTetramino('L', 0);
  } else if (!turnTetramino('l', 1)) {
    moveRight();
    turnTetramino('L', 0);
  } else if (!turnTetramino('l', -1)) {
    moveLeft();
    turnTetramino('L', 0);
  }
}

/**
 * @brief Rotates the current tetromino based on its type.
 *
 * This function checks the type of the current tetromino and applies the
 * appropriate rotation logic. It ensures that the rotation is possible without
 * collisions by checking the resulting positions.
 */
void rotate() {
  if (getData()->current_type == 'T' || getData()->current_type == 'L' ||
      getData()->current_type == 'J') {
    rotateLeft();
  } else if ((getData()->current_type == 'S' &&
              getData()->tetramino_current[1][3] == 1) ||
             (getData()->current_type == 'Z' &&
              getData()->tetramino_current[1][1] == 1)) {
    rotateLeft();
  } else if ((getData()->current_type == 'S' &&
              getData()->tetramino_current[1][3] == 0) ||
             (getData()->current_type == 'Z' &&
              getData()->tetramino_current[1][1] == 0)) {
    if (!turnTetramino('r', 0)) {
      turnTetramino('R', 0);
    } else if (!turnTetramino('r', 1)) {
      moveRight();
      turnTetramino('R', 0);
    } else if (!turnTetramino('r', -1)) {
      moveLeft();
      turnTetramino('R', 0);
    }
  } else if (getData()->current_type == 'I') {
    if (getData()->tetramino_current[2][0] == 1) {
      if (!turnTetramino('r', 0)) turnTetramino('R', 0);
    } else {
      if (!turnTetramino('l', 0)) {
        turnTetramino('L', 0);
      } else if (!turnTetramino('l', 1)) {
        moveRight();
        turnTetramino('L', 0);
      } else if (!turnTetramino('l', 2)) {
        moveRight();
        moveRight();
        turnTetramino('L', 0);
      } else if (!turnTetramino('l', -1)) {
        moveLeft();
        turnTetramino('L', 0);
      }
    }
  }
}

/**
 * @brief Tries to rotate the current tetromino in the given direction.
 *
 * @param direction The direction to rotate the tetromino ('L' for left, 'R' for
 * right).
 * @param x_pos The horizontal offset to check the rotation at a different
 * position.
 * @return int Returns 0 if rotation was successful, or a non-zero value if
 * rotation failed due to collision.
 */
int turnTetramino(char direction, int x_pos) {
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
          cant_turn = tmp_matrix[i][j] + getData()->field_simple[y][x] > 1
                          ? cant_turn + 1
                          : cant_turn;
        } else if (y > 19 || x < 0 || x > 9) {
          cant_turn = tmp_matrix[i][j] > 0 ? cant_turn + 1 : cant_turn;
        }
      } else if (direction == 'L' || direction == 'R') {
        getData()->tetramino_current[i][j] = tmp_matrix[i][j];
      }
    }
  }
  sumMatrix(&(getInfo()->field));
  return cant_turn;
}

/**
 * @brief Checks if the tetromino collides with other blocks on the field.
 *
 * @param tetramino The current tetromino to check for collision.
 * @param y_coord The y-coordinate of the tetromino.
 * @param x_coord The x-coordinate of the tetromino.
 * @return int Returns 1 if a collision occurs, 0 if no collision.
 */
int checkCollision(int **tetramino, int y_coord, int x_coord) {
  int lock = 0;
  for (int i = 0; i < TETRAMINO_SIZE && !lock; i++) {
    for (int j = 0; j < TETRAMINO_SIZE && !lock; j++) {
      int y = y_coord + i;
      int x = x_coord + j;

      if (y >= 0 && y < 20 && x >= 0 && x < 10) {
        lock = tetramino[i][j] + getData()->field_simple[y][x] > 1 ? lock + 1
                                                                   : lock;
      } else if (y > 19 || x < 0 || x > 9) {
        lock = tetramino[i][j] > 0 ? lock + 1 : lock;
      }
    }
  }
  return lock;
}

/**
 * @brief Checks if any lines are full and deletes them.
 *
 * If a line is full, it is deleted and all lines above it move down.
 * The score is updated accordingly for each line cleared.
 */
void checkFill() {
  int full_lines_count = 0;
  for (int i = 0; i < FIELD_HEIGHT; i++) {
    int is_full = 1;
    for (int j = 0; j < FIELD_WIDTH && is_full; j++) {
      if (getData()->field_simple[i][j] == 0) is_full = 0;
    }
    if (is_full) {
      deleteFullLine(i);
      full_lines_count++;
    }
  }
  sumMatrix(&(getInfo()->field));
  int score = 1;
  while (full_lines_count--) score *= 2;
  getInfo()->score += 100 * (score - 1);
  getInfo()->level = getInfo()->score / 600 + 1;
  if (getInfo()->level > 10) getInfo()->level = 10;
}

/**
 * @brief Deletes a full line and shifts all lines above it down.
 *
 * @param y_index The index of the line to be deleted.
 */
void deleteFullLine(int y_index) {
  for (int i = y_index; i > 0; i--) {
    for (int j = 0; j < FIELD_WIDTH; j++) {
      getData()->field_simple[i][j] = getData()->field_simple[i - 1][j];
    }
  }
  for (int j = 0; j < FIELD_WIDTH; j++) {
    getData()->field_simple[0][j] = 0;
  }
}