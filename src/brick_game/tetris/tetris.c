#include "./tetris.h"

/**
 * @brief Gets the current game information.
 *
 * This function initializes and retrieves the current game state, including the
 * game field, next piece, and other important parameters. If the field or next
 * piece has not been created, it attempts to create them. It also retrieves the
 * high score from a file if necessary.
 *
 * @return GameInfo_t* Pointer to the current game information structure.
 */
GameInfo_t *getInfo() {
  static GameInfo_t info = {NULL, NULL, 0, 0, 1, 1, 0};

  if (!info.field && info.pause != EXIT && info.pause != MEM_ERROR) {
    int tmp_high_score = 0;
    if (info.high_score < 1) {
      FILE *file = fopen("/usr/local/bin/brick_game_high_score.txt", "r");
      if (!file) {
        file = fopen("build/brick_game_high_score.txt", "r");
      }
      if (file) {
        fscanf(file, "%d", &tmp_high_score);
        if (info.high_score < tmp_high_score) info.high_score = tmp_high_score;
        fclose(file);
      }
    }
  }
  if (!(info.field) && (info.pause != EXIT && info.pause != MEM_ERROR)) {
    if (createMatrix(&(info.field), FIELD_HEIGHT, FIELD_WIDTH))
      info.pause = MEM_ERROR;
  }
  if (!(info.next) && (info.pause != EXIT && info.pause != MEM_ERROR)) {
    if (createMatrix(&(info.next), TETRAMINO_SIZE, TETRAMINO_SIZE))
      info.pause = MEM_ERROR;
  }
  return &info;
}

/**
 * @brief Gets the current field data.
 *
 * This function initializes and retrieves the current field data, which
 * includes the simplified game field and the current tetramino piece. If the
 * required matrices are not created, it attempts to create them.
 *
 * @return Field_data* Pointer to the current field data structure.
 */
Field_data *getData() {
  static Field_data data = {NULL, NULL, 0, 0, 0, 0};
  if (getInfo()->pause != EXIT && getInfo()->pause != MEM_ERROR) {
    if (!data.field_simple) {
      if (createMatrix(&(data.field_simple), FIELD_HEIGHT, FIELD_WIDTH))
        getInfo()->pause = MEM_ERROR;
    }
    if (!data.tetramino_current) {
      if (createMatrix(&(data.tetramino_current), TETRAMINO_SIZE,
                       TETRAMINO_SIZE))
        getInfo()->pause = MEM_ERROR;
    }
  }
  return &data;
}

/**
 * @brief Gets the current state of the game.
 *
 * This function returns a pointer to the current game state, which is a static
 * integer representing the current state of the game (e.g., MOVING, SPAWN,
 * GAME_OVER).
 *
 * @return int* Pointer to the current game state.
 */
int *getState() {
  static int state = START;
  return &state;
}

/**
 * @brief Updates the current game state.
 *
 * This function processes the current game state and updates it accordingly.
 * Depending on the state (e.g., MOVING, SHIFTING, SPAWN), it performs actions
 * such as moving pieces, spawning new pieces, or attaching the current piece to
 * the field. If the game is over, the result is saved.
 *
 * @return GameInfo_t Updated game information.
 */
GameInfo_t updateCurrentState() {
  if (getInfo()->pause != MEM_ERROR && getInfo()->pause != EXIT) {
    getData();
    getState();
    switch (*getState()) {
      case MOVING:
        timer();
        break;
      case SHIFTING:
        moveDown();
        break;
      case SPAWN:
        if (!spawn_next()) {
          *getState() = MOVING;
        } else {
          *getState() = GAME_OVER;
        }
        break;
      case ATTACHING:
        sumMatrix(&(getData()->field_simple));
        checkFill();
        *getState() = SPAWN;
        break;
      case GAME_OVER:
        saveResult();
        break;
      default:
        break;
    }
  } else {
    removeMatrix(&(getData()->tetramino_current), TETRAMINO_SIZE);
    removeMatrix(&(getData()->field_simple), FIELD_HEIGHT);
    removeMatrix(&(getInfo()->next), TETRAMINO_SIZE);
    removeMatrix(&(getInfo()->field), FIELD_HEIGHT);
  }
  return *(getInfo());
}

/**
 * @brief Processes the user input for various actions.
 *
 * This function handles the input from the user and updates the game state
 * based on the action performed. Depending on the action (e.g., Start, Pause,
 * Terminate), it can start the game, pause it, or terminate it. It also handles
 * the movement of tetrominoes (Left, Right, Down) and actions such as rotating
 * pieces.
 *
 * @param action The action to perform (e.g., Start, Pause, Left).
 * @param hold A flag indicating whether the action should be held or not (e.g.,
 * for continuous movement).
 */
void userInput(UserAction_t action, bool hold) {
  if (hold) *getState() = MOVING;
  switch (action) {
    case Start:
      if (*getState() == START) {
        createNext(&(getInfo()->next));
        *getState() = SPAWN;
      } else if (*getState() == PAUSE || *getState() == GAME_OVER) {
        saveResult();
        restartGame();
        createNext(&(getInfo()->next));
        *getState() = SPAWN;
      }
      break;
    case Pause:
      if (*getState() == PAUSE) {
        *getState() = MOVING;
        getInfo()->pause = 0;
      } else if (*getState() == MOVING) {
        *getState() = PAUSE;
        getInfo()->pause = 1;
      }
      break;
    case Terminate:
      saveResult();
      getInfo()->pause = EXIT;
      removeMatrix(&(getData()->tetramino_current), TETRAMINO_SIZE);
      removeMatrix(&(getData()->field_simple), FIELD_HEIGHT);
      removeMatrix(&(getInfo()->next), TETRAMINO_SIZE);
      removeMatrix(&(getInfo()->field), FIELD_HEIGHT);
      break;
    case Left:
      if (*getState() == MOVING) moveLeft();
      break;
    case Right:
      if (*getState() == MOVING) moveRight();
      break;
    case Down:
      if (*getState() == MOVING) moveDown();
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

/**
 * @brief Manages the game timer.
 *
 * This function calculates the time elapsed since the last move and triggers a
 * shift in the game state (i.e., moving the tetromino down) if the time exceeds
 * the speed threshold based on the current level.
 */
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

/**
 * @brief Saves the current game result.
 *
 * If the player's score exceeds the high score, it updates the high score file
 * to store the new value.
 */
void saveResult() {
  if (getInfo()->score > getInfo()->high_score) {
    FILE *file = fopen("/usr/local/bin/brick_game_high_score.txt", "r");
    if (file) {
      fclose(file);
      file = fopen("/usr/local/bin/brick_game_high_score.txt", "w");
    } else {
      file = fopen("build/brick_game_high_score.txt", "w");
    }
    if (file) {
      fprintf(file, "%d", getInfo()->score);
      fclose(file);
    }
  }
}