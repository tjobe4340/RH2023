#include <stdio.h>

char WALL[] = "\u258B"; // 'Left five eighths block' unicode character
char PLAYER[] = "\u2609"; // 'sun' unicode character
char TREASURE[] = "\u25A3"; // 'White square containing small black square' unicode character
char ENEMY[] = "\u25C8"; // 'White diamond containing small black diamond'
char HEALTH[] = "\u271A";

int main (){
    /*
    printf("foreground colors\n");
    for(int i = 30; i < 38; i++){
        printf("\033[5;%d;1mHello World\033[0m Nada\n", i);
    }
    printf("background colors\n");
    for(int i = 100; i < 107; i++){
        printf("\033[5;34;%dmHello World\033[0m Nada\n", i);
    }
    printf("styles\n");
    for(int i = 1; i < 8; i++){
        printf("\033[%d;36;1mHello World\033[0m Nada\n",i);
    }
    printf("fancy styling\n");
    for(int s = 0; s < 8; s++){
        for(int b = 0; b < 3; b++){
            for(int i = 0; i < 32; i++){
                printf("\033[38;5;%d;48;%d;%dmHi\033[0m ",i,s,b);
            }
            printf("\n");
        }
        printf("\n\n");
    }
    */
    for(int i = 0; i < 255; i++){
        if(i != 0 && i % 16 == 0){
            printf("\n");
        }
        printf("\033[38;5;%d;48;5;0m%s \033[0m",i, ENEMY);
    }
    printf("\n\033[38;5;29;48;5;0mTHIS IS A TEST\033[0m\n");
    printf("\033[38;5;95;48;5;0m%s\033[0m\n",WALL);
    printf("\033[38;5;31;48;5;0m%s\033[0m\n",PLAYER);
    printf("\033[38;5;1;48;5;0m%s\033[0m\n",HEALTH);
    printf("\033[38;5;64;48;5;0m%s\033[0m\n",ENEMY);
    printf("\033[38;5;143;48;5;0m%s\033[0m\n",TREASURE);
    return 0;
}