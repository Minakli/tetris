#include "./tetris.h"

GameInfo_t* getInfo() {
  static GameInfo_t game_info = {NULL, NULL, 0,-1,1,1,0};
  
  if(!(game_info.field)) create_matrix(&(game_info.field), FIELD_HEIGHT, FIELD_WIDTH);
  if(!(game_info.next)) create_matrix(&(game_info.next), TETRAMINO_SIZE, TETRAMINO_SIZE);
  if(game_info.high_score < 0) {
    game_info.high_score = 0;
    FILE *file = fopen("./brick_game_hight_score.txt", "r");
    if(file) {
      fscanf(file, "%d", &(game_info.high_score));
    }
    fclose(file);
  }
  return &game_info;
}

int* getState() {
  static int state = START;
  return &state;
}

GameInfo_t updateCurrentState() {
    switch (*getState())
    {
    case MOVING:
        /* code */
        break;
    case SHIFTING:
        /* code */
        break;
    case PAUSE:
        /* code */
        break;
    case GAME_OVER:
        /* code */
        break;
    // case /* constant-expression */:
    //     /* code */
    //     break;
    // case /* constant-expression */:
    //     /* code */
    //     break;
    
    default:
        break;
    }

    return *(getInfo());
}

void userInput(UserAction_t action, bool hold) {
           switch (action)
    {
        case Start:
            if(*getState() == START) *getState() = MOVING;
            break;
        case Pause:
            if(*getState() == MOVING) *getState() = PAUSE;
            if(*getState() == PAUSE) *getState() = MOVING;
            break;
        case Terminate:
            *getState() = GAME_OVER;
            break;
        case Left:
            move_left(hold, *getState(), *getInfo());
            break;
        case Right:
            move_right(hold, *getState(), *getInfo());
            break;
        case Down:
            move_down(hold, *getState(), *getInfo());
            break;
        case Action:
            rotate(hold, *getState(), *getInfo());
            break;
        default:
            break;
    }
}

void create_next_tetramino(int ***tetramino) {
  srand(time(NULL));
  static int prev_sign = -1;
  int sign = 0;
     do {
        sign = rand() % 7;
    } while (sign == prev_sign);
    prev_sign = sign;
  switch ("IOTLJSZ"[sign])
  {
  case 'I':
    for(int i = 0; i < 4; i++) *tetramino[1][i] = 1;
    break;
  case 'O':
    *tetramino[1][2] = 1;
    *tetramino[1][3] = 1;
    *tetramino[2][2] = 1;
    *tetramino[2][3] = 1;
    break;
  case 'T':
    *tetramino[1][2] = 1;
    for(int i = 1; i < 4; i++) *tetramino[2][i] = 1;
    break;
  case 'L':
    *tetramino[1][3] = 1;
    for(int i = 1; i < 4; i++) *tetramino[2][i] = 1;
    break;
  case 'J':
    *tetramino[1][1] = 1;
    for(int i = 1; i < 4; i++) *tetramino[2][i] = 1;
    break;
  case 'S':
    *tetramino[1][2] = 1;
    *tetramino[1][3] = 1;
    *tetramino[2][1] = 1;
    *tetramino[2][2] = 1;
    break;
  case 'Z':
    *tetramino[1][1] = 1;
    *tetramino[1][2] = 1;
    *tetramino[2][2] = 1;
    *tetramino[2][3] = 1;
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

void move_left(bool hold, int state, GameInfo_t info) {}
void move_right(bool hold, int state, GameInfo_t info) {}
void move_down(bool hold, int state, GameInfo_t info) {}
void rotate(bool hold, int state, GameInfo_t info) {}