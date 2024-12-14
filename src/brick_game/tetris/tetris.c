#include "tetris.h"

// static GameInfo_t game_info = {0};

GameInfo_t* getInfo() {
  static GameInfo_t game_info = {0};
  return &game_info;
}

int* getState(void) {
  static int state = START;
  return &state;
}

int *getEnd() {
  static int end = 0;
  return &end;
}

// Создаем пустое поле
// Выбираем рандомно текущую БАЗОВУЮ картинку (если нет next)
// Выбираем рандомно next
// Двигаем текущую картинку по полю, отправляя их сумму в главную структуру
// При столкновении или заполнении меняем БАЗОВУЮ картинку


Field_data* getFieldData() {
  static Field_data field_data = {0};
  field_data.field_simple = create_matrix(FIELD_WIDTH, FIELD_HEIGHT);
  field_data.tetramino_current = create_matrix(TETRAMINO_SIZE, TETRAMINO_SIZE);
  return &field_data;
}

// Сделать структуру: Поле без фигуры, фигура, ее координаты.

// Задача создавать картинку поля
void setField() {
  int** current_field = {0};
  int** current_tetramino ={0};
  int** field = getInfo()->field;


}

int** create_random_tetramino() {

}

// Меняет GameInfo_t при нажатии клавиши
void userInput(UserAction_t action, bool hold) {
  GameInfo_t *game_info = getInfo();
  int *state = getState();

//   typedef enum {
//   Start,
//   Pause,
//   Terminate,
//   Left,
//   Right,
//   Up,
//   Down,
//   Action
// } UserAction_t;

       switch (action)
    {
        case Start:
            press_start(hold, state);
            break;
        case Pause:
            press_pause(hold, state, game_info);
            break;
        case Terminate:
            press_terminate(state, game_info);
            break;
        case Left:
            move_left(hold, state, game_info);
            break;
        case Right:
            move_right(hold, state, game_info);
            break;
        case Down:
            move_down(hold, state, game_info);
            break;
        case Action:
            rotate(hold, state, game_info);
            break;
        default:
            break;
    }

}
// #define START 0
// #define SPAWN 1
// #define MOVING 2
// #define SHIFTING 3
// #define ATTACHING 4
// #define PAUSE 5

#define GAME_OVER 6
void press_start(bool hold, int *state) {
  if(*state == START) *state = SPAWN;
}

void press_pause(bool hold, int *state, GameInfo_t *game_info) {
  switch (*state)
  {
  case PAUSE:
    *state = game_info->pause;
    game_info->pause = 0;
    break;
  case SPAWN:
  case MOVING:
  case SHIFTING:
  case ATTACHING:
    game_info->pause = *state;
    *state = PAUSE;
    print_pause(game_info);
    break;
  default:
    break;
  }
}

void print_pause(GameInfo_t* game_info) {
// Указать координаты и текст
}

void press_terminate(int *state, GameInfo_t* game_info) {
  free_matrix(game_info->field, FIELD_HEIGHT);
  free_matrix(game_info->next, TETRAMINO_SIZE);
  int *end_pointer = getEnd();
  *end_pointer = 1;
}

void free_matrix(int **matrix, int rows) {
  for(int i = 0; i < rows; i++) {
    free(matrix[i]);
  }
  free(matrix);
}



int **create_matrix(int str, int col) {
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

int set_score() { return 0; }
int set_high_score() { return 0; }
int set_level() { return 0; }
int set_speed() { return 0; }
int set_pause() { return 0; }

on_start_state(sig, state) {
  {
    switch (sig)
    {
        case ENTER_BTN:
            *state = SPAWN;
            break;
        case ESCAPE_BTN:
            *state = EXIT_STATE;
            break;
        default:
            *state = START;
            break;
    }
}
}

GameInfo_t updateCurrentState() {
  // return game_info;
  return *(getInfo());
}




void setCurrentState() {
// Зависит от:
// - текущего состояния
// - действия игрока
// - событий в игре

}

// GameInfo_t qweqwe(GameInfo_t A) {
//   A.field = create_matrix(20, 10);
//   A.next = create_matrix(4, 4);
//   A.score = set_score();
//   A.high_score = set_high_score();
//   A.level = set_level();
//   A.speed = set_speed();
//   A.pause = set_pause();

//   return A;
// }
