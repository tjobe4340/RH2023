#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <signal.h>
#include <ctype.h>
#include <time.h>
#include <windows.h>


typedef struct Position{
    int x;
    int y;
}Position;

typedef struct Levels{
    char map[12][18];
    int level_num; //level progression
    Position start;
    struct Levels* next;
}Level;

typedef struct Monster{
    int health;
    int max_health;
    int type;
    int attack;
    Position loc;
}Monster;

typedef struct Loot{
    int loot;
}Loot;

typedef struct Player {
    int gold;
    Loot inv[10]; //malloc later
    Position loc;
    int level; //xp level
    int xp;
    int count;
    int health;
    int max_health;
    int running;
    int attack;//modify w/equipment later
}Player;

//RLite methods
Player player_init();
Monster monster_init();
Level* level_init();
void init_maps(Level*);
Player move_player(Player, Level* , char []);
void print_stats(Player);
void draw_map(Level*);
Player collect_treasure(Player, Level*);
Player battle(Player, Level*);
void print_player_battle_stats();
void print_enemy_battle_stats();
void cls_screen();
int empty_dungeon(Level*);
/*ideas methods
void dfs(int, int, char**, int**, int[], int, int );
void make_maze(Level*,int,int);*/

