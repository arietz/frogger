#include <time.h>
#include <stdlib.h>
#include <math.h>

//custom headers
#include "map_generation.h"
#include "config.h"
#include "entities.h"
#include "cars.h"

void map_reset(int ** grid, int ** lanes, EntityPlayer * player, EntityCar * cars, int * cars_num, int seed, Config * cfg, EntityStork * stork){
    clear_map(grid, lanes, cfg);
    remove_stork(stork);

    generate_lanes(seed, lanes, cfg);

    for (int i = 0; i < cfg->ROWS; i++){
        for (int j = 0; j < cfg->COLS; j++){
            if (lanes[i][0] == 1) grid[i][j] = 0;  //for road set grid to 0
            else grid[i][j] = lanes[i][0];         //forest = 0, river = -1, finish = 10
        }
    }

    generate_trees(seed, lanes, grid, cfg);
    generate_cars(lanes, grid, cars, cars_num, cfg);
    generate_boats(lanes, grid, cars, cars_num, cfg);
    spawn_stork(cfg, stork);

    //set player position
    player->col = cfg->COLS / 2;
    player->row = cfg->ROWS - 1;
    grid[player->row][player->col] = 3;
}

void clear_map(int ** grid, int ** lanes, Config * cfg){
    for (int i = 0; i < cfg->ROWS; i++)
    {
        for (int j = 0; j < cfg->COLS; j++)
        {
            grid[i][j] = 0;
        }
        lanes[i][0] = 0;
        lanes[i][1] = 0;
        lanes[i][2] = 0;
    }
}

void generate_trees(unsigned int seed, int ** lanes, int ** grid, Config * cfg){
    //if given seed, use it
    srand((seed != 0) ? seed : time(NULL));

    for(int i = 0; i < cfg->ROWS; i++){
        if (lanes[i][0] != 0) continue;

        for(int j = 0; j < cfg->COLS; j++){
            if (j == cfg->COLS / 2) continue;
            if (rand() % 100 <= cfg->TREE_CHANCE) grid[i][j] = 1; 
        }
    }
}

void generate_cars(int ** lanes, int ** grid, EntityCar * cars, int * cars_num, Config * cfg){
    for(int i = 0; i < cfg->ROWS; i++){
        if (lanes[i][0] != 1) continue;

        int counter = 0;
        int j = rand() % (cfg->COLS / 5) + 2;

        for(;j < cfg->COLS - 1; j += 5){
            if (rand() % 20 > 10 && counter < 3) {

                int values[6] = {i, j, lanes[i][1], lanes[i][2], 1, 1};
                init_car(&(cars[*cars_num]), values, cars_num);
                cars[*cars_num].type = 0;

                grid[i][j] = 1;
                *cars_num += 1;
                counter += 1;
            }
        }
    }
}

void generate_boats(int ** lanes, int ** grid, EntityCar * cars, int * cars_num, Config * cfg){
    for(int i = 0; i < cfg->ROWS; i++){
        if (lanes[i][0] != -1) continue;

        int counter = 0;
        int j = rand() % (cfg->COLS / 5) + 2;

        for(;j < cfg->COLS - 1; j += 5){
            if (rand() % 20 > 10 && counter < 3) {

                int values[6] = {i, j, lanes[i][1], lanes[i][2], 1, 1};
                init_car(&(cars[*cars_num]), values, cars_num);
                cars[*cars_num].type = 3;

                grid[i][j] = 2;
                *cars_num += 1;
                counter += 1;
            }
        }
    }
}

void generate_lanes(unsigned int seed, int ** lanes, Config * cfg){
    //if given seed, use it
    srand((seed != 0) ? seed : time(NULL));

    //static values of first and last lane
    lanes[0][0] = 10;      //finish 
    lanes[cfg->ROWS-1][0] = 0;  //start

    //combo system for higher chance of the same lane order
    int combo = 1;

    for(int i = 1; i < cfg->ROWS - 1; i++){  

        //new combo
        if(combo == 1){
            int temp = rand() % 6;

            //67% chance for road, 33% chance for river
            if(temp <= 3) lanes[i][0] = 1;         //road
            else lanes[i][0] = cfg->RIVERS ? -1 : 1;                 //river

            combo++;
        }
        //check if combo persists and it isn't forest combo
        else if(rand() % 16 > pow(2,combo) && lanes[i-1][0] != 0){
            lanes[i][0] = lanes[i-1][0];
            combo++;
        }
        //combo is broken, for gameplay's sake we set lane to forest
        else {
            lanes[i][0] = 0;
            combo = 1;
        }
    }

    fill_lanes(lanes, cfg);
}

void fill_lanes(int ** lanes, Config * cfg){
    for (int i = 0; i < cfg->ROWS; i++)
    {
        //road
        if (lanes[i][0] == 1){
            //random direction
            lanes[i][1] = rand() % 2;

            //lane speed
            lanes[i][2] = (rand() % 7) + 4;
        }
        //river
        else if (lanes[i][0] == -1){
            //opposite direction to previous if it exists
            if (lanes[i-1][0] == -1) lanes[i][1] = abs(lanes[i-1][1] - 1) ;  
            else lanes[i][1] = rand() % 2;
            
            //lane speed
            lanes[i][2] = (rand() % 5) + 20;
        }
    }         
}

//spawns new cars and boats
void spawn_car(int ** lanes, int ** grid, int * cars_num, EntityCar * cars, Config * cfg){
    //select random row
    int row = (rand() % (cfg->ROWS - 1)) + 1;

    if (abs(lanes[row][0]) == 1){
        int col = (lanes[row][1]) ? 0 : cfg->COLS - 1;

        //if spawn place is empty
        if (grid[row][col] == 0 || grid[row][col] == -1){

            //if something spawned on this row lately, dont spawn
            int second = (col == 0) ? col + 1 : col - 1;
            int third = (col == 0) ? col + 2 : col - 2;

            if (((grid[row][second] == 0 && grid[row][third] == 0) || (grid[row][second] == -1 && grid[row][third] == -1))) {
                int values[6] = {row, col, lanes[row][1], lanes[row][2], 1, 1};
                init_car(&cars[*cars_num], values);

                //car type
                if (lanes[row][0] == -1) cars[*cars_num].type = 3;
                else cars[*cars_num].type = car_type();

                grid[row][col] += (cars[*cars_num].type == 0) ? 1 : cars[*cars_num].type;

                *cars_num += 1;
            }
        }
    }
}

int car_type(){
    int type = (rand() % 25 == 0) ? 1 : 0; // friendly
    type = (rand() % 25 == 0) ? 2 : type; // taxi

    return type;
}

void spawn_stork(Config * cfg, EntityStork * stork){
    int row = rand() % cfg->ROWS;
    int col = (rand() % 2) ? 0 : cfg->COLS - 1;

    stork->row = row;
    stork->col = col;
    stork->prev_col = col;
    stork->prev_row = row;
    stork->exists = 1;
}

void remove_stork(EntityStork * stork){
    stork->row = 0;
    stork->col = 0;
    stork->prev_col = 0;
    stork->prev_row = 0;
    stork->exists = 0;
}