#include "RLite.h"

Player player_init(){
    Player player;
    player.loc;
    player.loc.x = 1;
    player.loc.y = 3;
    player.gold = 0;
    player.health = 100;
    player.max_health = 100;
    player.attack = 10;
    player.xp = 0;
    player.level = 1;
    return player;
}
Monster monster_init(){
    Monster monster;
    monster.type = (rand()) % 20 + 1;
    monster.health = 20 + monster.type;
    monster.max_health = monster.health;
    monster.attack = monster.type + 9;

    return monster;
}
Level* level_init(){
    Level* level = (Level*)malloc(sizeof(Level));
    level->level_num = 1;
    level->next = NULL;
    level->start.x = 1;
    level->start.y = 3;
    init_maps(level);
    return level;
}

void init_maps(Level* levels){
    //Level1
    strcpy(levels->map[0], "################");
    strcpy(levels->map[1], "#   E          #");
    strcpy(levels->map[2], "#  ### #########");
    strcpy(levels->map[3], "#P #  E    T  ##");
    strcpy(levels->map[4], "#  #T    # ##  #");
    strcpy(levels->map[5], "#  ####### HH E#");
    strcpy(levels->map[6], "####       E   #");
    strcpy(levels->map[7], "#       ########");
    strcpy(levels->map[8], "#   H   H   H  #");
    strcpy(levels->map[9], "#   E   E   E  #");
    strcpy(levels->map[10], "#              #");
    strcpy(levels->map[11], "################");
}
    

int main(int argc, char* argv[]){

    char answer[10];

    Player player = player_init();
    Level* level = level_init();

    cls_screen();
    print_stats(player);
    draw_map(level);

    while (1)
    { 
        fflush(stdin);
        printf("%s\n", "Make your move");
        scanf("%10s", answer);
        printf("Answer: %s\n", answer);
        cls_screen();
        if(answer[0] == 'q'){
            break;
        }

        player = move_player(player, level, answer);
        cls_screen();
        print_stats(player);
        draw_map(level);
    }
    free(level);
    /*make an array of floors that have a random maze layout
    *add enemies randomly throught the floor, along with prizes and powerups*/
    printf("exiting");
    return(0);
}
Player move_player(Player player, Level* level, char answer[]){
    Position diff;
    diff.x = 0;
    diff.y = 0;
    switch (answer[0])
    {
    case 'w':
        //move up
        diff.y = -1;
        break;
    case 'a':
        //move left
        diff.x = -1;
        break;
    case 's':
        //move down
        diff.y = 1;
        break;
    case 'd':
        //move right
        diff.x = 1;
        break;
    default:
        printf("invalid input");
        return player;
    }
    Position temp = player.loc;
    temp.x += diff.x;
    temp.y += diff.y;
    if(level->map[temp.y][temp.x] != '#' ){
        if(level->map[temp.y][temp.x] == 'T'){
            //calling treasure
            //printf("\n\nTest\n\n");
            player = collect_treasure(player, level);
        }
        else if(level->map[temp.y][temp.x] == 'E'){
            //calling fight
            player = battle(player,level);
        }
        else if(level->map[temp.y][temp.x] == 'H'){
            //player lands on heal pad
            player.health += rand() % 15 + 5;
        }
        level->map[player.loc.y][player.loc.x] = ' ';
        level->map[temp.y][temp.x] = 'P';
        player.loc = temp;
    }
    return player;
}
void print_stats(Player player){
    printf("HEALTH: %3d\tGOLD: %5d\n", player.health, player.gold);
}
void draw_map(Level* level){
    int i;
    for(i = 0; i < 12; i ++)
        printf("%s\n", level->map[i]);

}
Player collect_treasure(Player player, Level* level){
    int treasure = rand() % (int)(pow(level->level_num,(2.0/3.0))) + 10;
    player.gold += treasure;
    return player;
}
int run_away(Level* level, Player player){
    Position diff;
    diff.x = rand() % 3 - 1;
    diff.y = rand() % 3 - 1;
    Position temp = player.loc;
    temp.x += diff.x;
    temp.y += diff.y;
    if(level->map[temp.y][temp.x] != '#' ){
        player.loc = temp;
        return 1;
    }else{
        return 0;
    }
}
Player battle(Player player, Level* level){
    //define monster type
    Monster monster = monster_init();
    int pAttack;
    int mAttack;
    char answer[10];

    do {
        //ask
        cls_screen();
        print_player_battle_stats(player);
        print_enemy_battle_stats(monster);
        answer[0] = '\0';
        pAttack = (rand() % 10 + player.attack);// % (rand()%4)); //tweak as needed
        mAttack = (rand()) % 10 + monster.attack;
        //fflush(stdin);
        printf("Do you wish to run away or attack? (r/a)\n");
        scanf("%5s", answer);
        cls_screen();
        //printf("test\n\n\n\n\n\n");
        switch (answer[0]){
            case 'r':
                //return true >1 or false
                if(run_away(level, player)){
                    printf("Successfully ran");
                    return player;
                }
                else{
                    //player.health -= 10;
                    printf("Failed to run");
                }
                break;
            case 'a':
                monster.health -= pAttack;
                printf("Player did %d damage!\n", pAttack);
                break;
            default:
                printf("invalid command");
                continue;
                break;
        }
        if (player.health <= 0){
            player.health=0;
            cls_screen();
            print_player_battle_stats(player);
            print_enemy_battle_stats(monster);
            printf("Oh no! You died!\n");
            char temp[10];
            printf("Press q and then press enter to exit game\n");
            scanf("%s", temp);
            exit(-1);
        }
        //sleep
        //sleep(0.5);
        if (monster.health <= 0){
            monster.health=0;
            printf("You did it! The monster is dead!\n");
            player.xp += monster.type;
            break;
        }
        //attack
        player.health -= mAttack;
        if (player.health <= 0){
            player.health=0;
            cls_screen();
            print_player_battle_stats(player);
            print_enemy_battle_stats(monster);
            printf("Oh no! You died!\n");
            char temp[10];
            printf("Press q and then press enter to exit game\n");
            scanf("%s", temp);
            exit(-1);
        }
        printf("Monster did %d damage!\n", mAttack);
    } while (player.health > 0 && monster.health > 0);
    printf("Escaped out of battle\n");
    if (player.xp>=100){ //can adjust based on diff
        printf("Huzah! You've leveled up! You are now level %d!\n", (player.level+1));
        player.xp -= 100;
    }
    printf("You found %d gold!\n", monster.type);
    player.gold += monster.max_health;
    return player;
}
void print_player_battle_stats(Player player){
    printf(" PLAYER HEALTH %3d\n", player.health);
    printf("+--------------------+\n");
    printf("|");
    int i;
    for(i=0; i < (player.health*20.0/player.max_health); i++){
        printf("-");
    }
    for(i=0; i < (20-(player.health*20.0/player.max_health)); i++){
        printf(" ");
    }
    printf("|\n");
    printf("+--------------------+\n\n");
    //printf(" Attack: %d\n", attack);
    //printf(" Defense: %d\n", defense);
}
void print_enemy_battle_stats(Monster monster){
    printf(" You have encounterd a %d class monster!\n", monster.type );
    printf(" MONSTER HEALTH: %3d\n", monster.health);
    printf("+--------------------+\n");
    printf("|");
    int i;
    for(i=0; i < (monster.health*20.0/monster.max_health); i++){
        printf("-");
    }
    for(i=0; i < (20-(monster.health*20.0/monster.max_health)); i++){
        printf(" ");
    }
    printf("|\n");
    printf("+--------------------+\n\n");
    //printf(" Attack: %d\n", attack);
    //printf(" Defense: %d\n", defense);
}

void cls_screen(){
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
}