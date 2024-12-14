#include "main.h"

int main(){
    // print_game();
    printf("qwe\n");
    return 0;
}

void print_game(){
    for(int i = 0; i < 1000; i++){
        // print_shot_field();
    }   
}

void print_shot_field(){
    GameInfo_t current_info = updateCurrentState();
    for(int i = 0; i < 20; i++){
        for(int j = 0; j < 10; j++){
            printf("%d", current_info.field[i][j]);
        }
        printf("/n");

    }
}