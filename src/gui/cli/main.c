#include "main.h"

/**
 * @brief Main function to start the game, handle user input and manage game
 * state.
 *
 * Initializes ncurses environment, sets up key bindings, colors, and enters the
 * game loop to process user inputs and update game state. The loop continues
 * until the user terminates the game.
 *
 * @return int Returns 0 if the game ended normally, otherwise returns an error
 * code.
 */
int main() {
  int err = -1;

  initscr();
  cbreak();
  noecho();
  curs_set(0);
  timeout(0);
  keypad(stdscr, TRUE);
  start_color();
  init_pair((short)1, COLOR_BLACK, COLOR_YELLOW);
  init_pair((short)2, COLOR_BLACK, COLOR_WHITE);

  while (err < 0) {
    int ch = getch();
    switch (ch) {
      case '\n':
        userInput(Start, false);
        break;
      case KEY_LEFT:
      case 'a':
      case 'A':
        userInput(Left, false);
        break;
      case KEY_RIGHT:
        userInput(Right, false);
        break;
      case KEY_UP:
        userInput(Up, false);
        break;
      case KEY_DOWN:
        userInput(Down, false);
        break;
      case ' ':
        userInput(Action, false);
        break;
      case 'p':
        userInput(Pause, false);
        break;
      case 'q':
        userInput(Terminate, false);
        err = 0;
        break;
      default:
        break;
    }
    napms(5);
    refresh();
    err = printGame(updateCurrentState());
  }
  endwin();
  return err;
}

/**
 * @brief Prints the current game information on the screen.
 *
 * Displays game speed, level, score, and top score. Also updates the game field
 * and shows the next piece preview. Depending on the current game state (pause,
 * game over), relevant messages are shown.
 *
 * @param info Structure containing the current game state (speed, level, score,
 * etc.).
 * @return int Returns 0 if the game ended, otherwise returns 1 for continue
 * playing.
 */
int printGame(GameInfo_t info) {
  int terminate = -1;
  if (info.pause == MEM_ERROR) {
    terminate = 1;
  } else if (info.pause == EXIT) {
    terminate = 0;
  } else {
    attron(COLOR_PAIR(2));
    move(1, 22);
    for (int i = 0; i < 20; i++) {
      move(i + 1, 22);
      for (int j = 0; j < 16; j++) {
        printw(" ");
      }
    }
    mvprintw(9, 25, "SPEED:");
    mvprintw(10, 25, "%d ms", info.speed);
    mvprintw(12, 25, "LEVEL:");
    mvprintw(13, 25, "%d", info.level);
    mvprintw(15, 25, "SCORE:");
    mvprintw(16, 25, "%d", info.score);
    mvprintw(18, 25, "TOP SCORE:");
    mvprintw(19, 25, "%d", info.high_score);
    attroff(COLOR_PAIR(2));
    printField(info.field);
    printNext(info.next);
    if (info.pause == 1) {
      attron(COLOR_PAIR(2));
      mvprintw(10, 5, "<< PAUSE >>");
      mvprintw(11, 2, "Press p for resume");
      mvprintw(12, 10, "or ");
      mvprintw(13, 4, "Enter for next");
      attroff(COLOR_PAIR(2));
    }
    if (info.pause == GAME_OVER) {
      attron(COLOR_PAIR(2));
      mvprintw(10, 3, "<< GAME OVER >>");
      mvprintw(11, 1, "Press Enter for next");
    }
  }
  return terminate;
}

/**
 * @brief Prints the game field on the screen.
 *
 * Loops through the field and prints each cell. Cells with a value greater than
 * 0 are drawn in the active color, while others are printed with a default
 * color.
 *
 * @param field A 2D array representing the game field.
 */
void printField(int **field) {
  if (field) {
    for (int i = 0; i < 20; i++) {
      move(i + 1, 1);
      for (int j = 0; j < 10; j++) {
        if (field[i][j] > 0) {
          attron(COLOR_PAIR(1));
          printw("  ");
          attroff(COLOR_PAIR(1));
        } else {
          attron(COLOR_PAIR(2));
          printw("  ");
          attroff(COLOR_PAIR(2));
        }
      }
    }
  }
}

/**
 * @brief Prints the next piece preview.
 *
 * Displays the next piece that will appear in the game using a 5x5 grid.
 *
 * @param next A 2D array representing the next piece.
 */
void printNext(int **next) {
  if (next) {
    for (int i = 0; i < 5; i++) {
      move(i + 2, 25);
      for (int j = 0; j < 5; j++) {
        if (next[i][j] == 1) {
          printw("  ");
        } else {
          attron(COLOR_PAIR(2));
          printw("  ");
          attroff(COLOR_PAIR(2));
        }
      }
    }
  }
}