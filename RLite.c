#include "RLite.h"

char WALL[] = "\u258B"; // 'Left five eighths block' unicode character
char PLAYER[] = "\u2609"; // 'sun' unicode character
char TREASURE[] = "\u25A3"; // 'White square containing small black square' unicode character
char ENEMY[] = "\u25C8"; // 'White diamond containing small black diamond' unicode character
char HEALTH[] = "\u271A"; // 'Heavy Greek cross' unicode character

char WALL_COLOR[] = "\033[38;5;95;48;5;0m";
char PLAYER_COLOR[] = "\033[38;5;31;48;5;0m";
char TREASURE_COLOR [] = "\033[38;5;143;48;5;0m";
char ENEMY_COLOR[] = "\033[38;5;64;48;5;0m";
char HEALTH_COLOR[] = "\033[38;5;1;48;5;0m";
char CLEAR_COLOR[] = "\033[0m";
//possible enemies ⍒,☠

/*
    initialize player
    starting health = 100 
    starting level = 1 
    Base attack is 10
*/
Player player_init(){
    Player player;
    player.gold = 0;
    player.health = 100;
    player.max_health = 100;
    player.attack = 10;
    player.xp = 0;
    player.level = 1;
    player.count=0;
    return player;
}
/*
    Monster Initialization
    set the difficlty to 1-20
    Base health = 20 
    Base attack = 9
    add difficulty to all stats
*/
Monster monster_init(){
    Monster monster;
    //The difficulty of the monster to scale attack/health
    monster.type = (rand()) % 20 + 1;
    monster.health = 20 + monster.type;
    monster.max_health = monster.health;
    monster.attack = monster.type + 9;
    return monster;
}
/*
    choose the map and choose player starting position
*/
Level* level_init(){
    Level* level = (Level*)malloc(sizeof(Level));
    level->level_num = 1;
    level->next = NULL;
    level->start.x = 1;
    level->start.y = 3;
    init_maps(level);
    return level;
}

/*
    Design the map
*/
void init_maps(Level* levels){
    int dungeon = rand() % 1;
    switch(dungeon){
        case 0:
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
            break;
    }
}

int main(int argc, char* argv[]){
    //choose a buffer for user response
    char answer[10];
    //set random seed to time to increase randomness
    srand(time(NULL));
    //initialize all variables
    Player player = player_init();
    Level* level = level_init();
    //initialize player starting position in respect to level
    player.loc.x = level->start.x;
    player.loc.y = level->start.y;
    cls_screen();
    //print screen
    print_stats(player);
    draw_map(level);

    while (1)
    { 
        //ask player for movement
        fflush(stdin);
        printf("%s\n", "Make your move");
        scanf("%10s", answer);
        printf("Answer: %s\n", answer);
        cls_screen();
        //quit game
        if(answer[0] == 'q'){
            break;
        }
        //move player if possible and do appropriate actions
        player = move_player(player, level, answer);
        cls_screen();
        //print screen
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
    //save temporary location of where player would move
    Position temp = player.loc;
    temp.x += diff.x;
    temp.y += diff.y;
    //see if position where moving too is not blank and not a wall
    if(level->map[temp.y][temp.x] != '#' ){
        if(level->map[temp.y][temp.x] == 'T'){
            //calling treasure
            player = collect_treasure(player, level);
        }
        else if(level->map[temp.y][temp.x] == 'E'){
            //init boolean for if ran away was successful
            player.running = 0;
            //calling fight
            player = battle(player,level);
            //run away was successful
            if(player.running == 1){
                //break out of movement and stay where player was
                //before the fight occurred
                return player;
            }
        }
        else if(level->map[temp.y][temp.x] == 'H'){
            //player lands on heal pad
            player.health += rand() % 15 + 5;
        }
        //set position where player was to blank
        level->map[player.loc.y][player.loc.x] = ' ';
        //adjust player position
        level->map[temp.y][temp.x] = 'P';
        player.loc = temp;

        //if there are no more enemies, you've won
        if(empty_dungeon(level) == 0){
            printf("Congrats! You slayed all the enemies\n");
            char temp[10];
            printf("Press q and then press enter to exit game\n");
            scanf("%s", temp);
            free(level);
            exit(-1);
        }
    }
    return player;
}
//print player stats
void print_stats(Player player){
    printf("HEALTH: %3d\tGOLD: %5d\n", player.health, player.gold);
}
//draw the map
//take the ascii representation and convert to colored unicode
void draw_map(Level* level){
    int i;
    //for each cell in the map, convert the basic format to the respective unicode characters
    //set the text color, draw the unicode, then clear the text formatting
    for(i = 0; i < 12; i ++){
        for(int j = 0; j < 18; j++){
            switch(level->map[i][j]){
                case 'P':
                    printf("%s%s%s", PLAYER_COLOR, PLAYER, CLEAR_COLOR);
                    break;
                case 'T':
                    printf("%s%s%s", TREASURE_COLOR, TREASURE, CLEAR_COLOR);
                    break;
                case 'E':
                    printf("%s%s%s", ENEMY_COLOR, ENEMY, CLEAR_COLOR);
                    break;
                case 'H':
                    printf("%s%s%s", HEALTH_COLOR, HEALTH, CLEAR_COLOR);
                    break;
                case '#':
                    printf("%s%s%s", WALL_COLOR, WALL, CLEAR_COLOR);
                    break;
                //default is printing a ' ' (space) with black background and foreground
                default:
                    printf("\033[38;5;0;48;5;0m \033[0m");
            }
        }
        printf("\n");
    }

}
//Collecting gold from treasure spaces
Player collect_treasure(Player player, Level* level){
    //depending on how many rooms the player has cleared, the player gets more gold
    int treasure = rand() % (int)(pow(level->level_num,(2.0/3.0))) + 10;
    player.gold += treasure;
    return player;
}
//method for determining if player can run away
int run_away(Level* level, Player player){
    Position diff1;
    // choose a random orthagonal position
    diff1.x = rand() % 3 - 1;
    diff1.y = rand() % 3 - 1;
    Position temp1 = player.loc;
    temp1.x += diff1.x;
    temp1.y += diff1.y;
    //see if position player is running to is not the enemy and not a wall
    if(level->map[temp1.y][temp1.x] != '#' && level->map[temp1.y][temp1.x] != 'E'){
        //run away was successful
        return 1;
    }else{
        //run away failed
        return 0;
    }
}
//battle between monster and player
Player battle(Player player, Level* level){
    //define monster type
    Monster monster = monster_init();
    //store player attack
    int pAttack;
    //store monster attack
    int mAttack;
    //store buffer for user input
    char answer[10];

    do {
        //print battle screen
        cls_screen();
        print_player_battle_stats(player);
        print_enemy_battle_stats(monster);
        //clear buffer
        answer[0] = '\0';
        //randomize both player and monster attack
        pAttack = rand() % 10 + player.attack;// player attack = base + 0-9
        mAttack = rand() % 10 + monster.attack;// monster attack = base + 0-9
        //prompt user for an action
        printf("Do you wish to run away or attack? (r/a)\n");
        scanf("%5s", answer);
        
        cls_screen();
        switch (answer[0]){
            //if player attempts to run away...
            case 'r':
                //run away successful
                if(run_away(level, player)){
                    printf("Successfully ran");
                    //return true to parent function (movement)
                    player.running = 1;
                    return player;
                }
                else{
                    //continue through function to take damage from monster's turn
                    printf("Failed to run");
                }
                break;
            //if player attempts to attack...
            case 'a':
                //monster takes damage equal to player's attack
                monster.health -= pAttack;
                printf("Player did %d damage!\n", pAttack);
                break;
            //if response was not r or a
            default:
                printf("invalid command");
                continue;
                break;
        }
        //if monster dies after player attacks, monster does not get to attack back
        if (monster.health <= 0){
            monster.health=0;
            printf("You did it! The monster is dead!\n");
            //xp is gained equal to monster's difficulty
            player.xp += monster.type;
            break;
        }

        //monster attacks player
        player.health -= mAttack;

        //if player dies
        if (player.health <= 0){
            player.health=0;
            //death screen is shown
            cls_screen();
            print_player_battle_stats(player);
            print_enemy_battle_stats(monster);
            printf("Oh no! You died!\n");
            //waiting for user response to end the game
            char temp[10];
            printf("Press q and then press enter to exit game\n");
            scanf("%s", temp);
            free(level);
            exit(-1);
        }
        printf("Monster did %d damage!\n", mAttack);
        //repeat battle while both player and monster are alive
    } while (player.health > 0 && monster.health > 0);

    //if player xp >= 100, then they level up
    if (player.xp>=100){ //TODO: can adjust based on diff
        printf("Huzah! You've leveled up! You are now level %d!\n", (player.level+1));
        player.xp -= 100;
    }

    //gold gained equal to monster's difficulty
    printf("You found %d gold!\n", monster.type);
    player.gold += monster.type;
    return player;
}
//print the battle stats of player
void print_player_battle_stats(Player player){
    //print player health title
    printf(" PLAYER HEALTH %3d\n", player.health);
    //print player health bar
    printf("+--------------------+\n");
    printf("|");
    for(int i=0; i < min(player.health*20.0/player.max_health,20); i++){
        printf("-");
    }
    for(int i=0; i < max(20-(player.health*20.0/player.max_health),0); i++){
        printf(" ");
    }
    printf("|\n");
    printf("+--------------------+\n\n");
}
//print the battle stats of the monster
void print_enemy_battle_stats(Monster monster){
    //print the difficulty of the monster
    printf(" You have encounterd a %d class monster!\n", monster.type);
    //print the health of the monster
    printf(" MONSTER HEALTH: %3d\n", monster.health);
    //print the monster's health bar
    printf("+--------------------+\n");
    printf("|");
    for(int i=0; i < (monster.health*20.0/monster.max_health); i++){
        printf("-");
    }
    for(int i=0; i < (20-(monster.health*20.0/monster.max_health)); i++){
        printf(" ");
    }
    printf("|\n");
    printf("+--------------------+\n\n");
}
//function to count how many enemies remain in the dungeon
int empty_dungeon(Level* level){
    int enemies = 0;
    for(int i=0;i<12;i++){
        for(int j=0;j<18;j++){
            if(level->map[i][j] == 'E'){
                enemies+=1;
            }
        }
    }
    return enemies;
}
//'clear' screen by flooding terminal buffer with new lines
void cls_screen(){
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
}
//replace windows.h function to increase mac usability
//return minimum of two integers
int min(int a, int b){
    return (a < b ? a : b);
}
//replace windows.h function to increase mac usability
//return max of two integers
int max(int a, int b){
    return (a > b ? a : b);
}