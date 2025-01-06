#include "main.h"

int main(){
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    timeout(0);
    keypad(stdscr, TRUE);
    start_color();
    init_pair((short)1, COLOR_BLACK, COLOR_YELLOW);
    init_pair((short)2, COLOR_BLACK, COLOR_WHITE);
    

    int end = 1;
    while(end) {
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
                end = 0;
                break;
            default:
                break;
        }
    // clear();  
    // usleep(1000);
    napms(5);
    refresh();
    print_game(updateCurrentState());
    }
    endwin();
    // free();
    return end;
}

void print_game(GameInfo_t info){
        attron(COLOR_PAIR(2));
        move(1,22);
    for(int i = 0; i < 20; i++) {
        move(i + 1, 22);
        for(int j = 0; j < 16; j++) {
            printw(" ");
        }
    }
    mvprintw(11, 25, "LEVEL:");
    mvprintw(12, 25, "%d", info.level);
    mvprintw(14, 25, "SCORE:");
    mvprintw(15, 25, "%d", info.score);
    mvprintw(17, 25,"TOP SCORE:");
    mvprintw(18, 25,"%d", info.high_score);
    attroff(COLOR_PAIR(2));
    print_field(info.field);
    print_next(info.next);
}

void print_field(int **field ){
    for(int i = 0; i < 20; i++){
        move(i + 1, 1);
        for(int j = 0; j < 10; j++){
            if(field[i][j] > 0) {
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

void print_next(int **next){
    for(int i = 0; i < 5; i++){
        move(i + 2, 25);
        for(int j = 0; j < 5; j++){
            if(next[i][j] == 1) {
                printw("  ");
            } else {
                attron(COLOR_PAIR(2));
                printw("  ");
                attroff(COLOR_PAIR(2));
            }
        }
    }
}