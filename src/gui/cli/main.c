#include "main.h"

int main(){
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    timeout(0);
    start_color();
    init_pair((short)1, COLOR_BLACK, COLOR_YELLOW);
    init_pair((short)2, COLOR_WHITE, COLOR_BLACK);
    // print_menu();

    int end = 1;
    while(end) {
        int ch = getch();
        switch (ch) {
            case '\n':
                userInput(Start, false);
                break;
            case KEY_LEFT:
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
                end = 0;
                break;
            default:
                break;
        }
    print_game(updateCurrentState());
    refresh();
    }
    endwin();
    // free();
    return end;
}

void print_game(GameInfo_t info){
    print_field(info.field);
    for(int i = 0; i < 20; i++) {
        move(i, 22);
        attron(COLOR_PAIR(2));
        for(int j = 0; j < 15; j++) {
            printw(" ");
        }
        attroff(COLOR_PAIR(2));
    }
    print_next(info.next);
    mvprintw(9, 25, "LEVEL: %d", info.level);
    mvprintw(13, 25, "SCORE: %d", info.score);
    mvprintw(18,25,"TOP SCORE: %d", info.high_score);
}

void print_field(int **field ){
    move(0, 5);
    for(int i = 0; i < 20; i++){
        for(int j = 0; j < 10; j++){
            if(field[i][j] == 1) {
                attron(COLOR_PAIR(1));
                printw(" ");
                attroff(COLOR_PAIR(1));
            } else {
                attron(COLOR_PAIR(2));
                printw(" ");
                attroff(COLOR_PAIR(2));
            }
        }
        move(i, 5);

    }
}

void print_next(int **next){
    int next_x = 13;
    int next_y = 0;
    move(next_y, next_x);
    for(int i = 0; i < 20; i++){
        for(int j = 0; j < 10; j++){
            if(next[i][j] == 1) {
                attron(COLOR_PAIR(1));
                printw(" ");
                attroff(COLOR_PAIR(1));
            } else {
                printw(" ");
            }
        }
        move(i, 13);

    }
}