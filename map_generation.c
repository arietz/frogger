#include <time.h>
#include <stdlib.h>
#include <math.h>

//custom headers
#include "map_generation.h"
#include "config.h"
#include "entities.h"
#include "cars.h"

void map_reset(int grid[ROWS][COLS], int lanes[ROWS][3], EntityPlayer * player, EntityCar cars[MAX_CARS], int * cars_num, int seed){
    clear_map(grid, lanes);

    //set player position
    player->col = COLS / 2;
    player->row = ROWS - 1;

    generate_lanes(seed, lanes);

    for (int i = 0; i < ROWS; i++){
        for (int j = 0; j < COLS; j++){
            if (lanes[i][0] == 1) grid[i][j] = 0;  //for road set grid to 0
            else grid[i][j] = lanes[i][0];         //forest = 0, river = -1, finish = 10
        }
    }

    generate_trees(seed, lanes, grid);
    generate_cars(seed, lanes, grid, cars, cars_num);
}

void clear_map(int grid[ROWS][COLS], int lanes[ROWS][3]){
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            grid[i][j] = 0;
        }
        lanes[i][0] = 0;
        lanes[i][1] = 0;
        lanes[i][2] = 0;
    }
    
}

void generate_trees(unsigned int seed, int lanes[ROWS][3], int grid[ROWS][COLS]){
    //if given seed, use it
    srand((seed != 0) ? seed : time(NULL));

    for(int i = 0; i < ROWS; i++){
        if (lanes[i][0] != 0) continue;

        for(int j = 0; j < COLS; j++){
            if (rand() % 20 <= 5) grid[i][j] = 1; 
        }
    }

    //clear space for starting player
    grid[ROWS - 1][COLS / 2] = 0;
}

void generate_cars(unsigned int seed, int lanes[ROWS][3], int grid[ROWS][COLS], EntityCar cars[], int * cars_num){
    //if given seed, use it
    srand((seed != 0) ? seed : time(NULL));

    for(int i = 0; i < ROWS; i++){
        if (lanes[i][0] != 1) continue;

        int counter = 0;
        int j = rand() % (COLS / 5) + 2;

        for(;j < COLS - 1; j += 5){
            if (rand() % 20 > 10 && counter < 3) {
                int values[6] = {i, j, lanes[i][1], lanes[i][2], 1, 1};
                init_car(&(cars[*cars_num]), values, cars_num);
                grid[i][j] = 1;
                *cars_num += 1;
                counter += 1;
            }
        }
    }
}

void generate_lanes(unsigned int seed, int lanes[ROWS][3]){
    //if given seed, use it
    srand((seed != 0) ? seed : time(NULL));

    //static values of first and last lane
    lanes[0][0] = 10;      //finish 
    lanes[ROWS-1][0] = 0;  //start

    //combo system for higher chance of the same lane order
    int combo = 1;

    for(int i = 1; i < ROWS - 1; i++){  

        //new combo
        if(combo == 1){
            int temp = rand() % 6;

            //67% chance for road, 33% chance for river
            if(temp <= 3) lanes[i][0] = 1;         //road
            else lanes[i][0] = 1;                 //river

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

    fill_lanes(lanes);
}

void fill_lanes(int lanes[ROWS][3]){
    for (int i = 0; i < ROWS; i++)
    {
        //road
        if (lanes[i][0] == 1){
            //random direction
            lanes[i][1] = rand() % 2;

            //lane speed
            lanes[i][2] = (rand() % 10) + 3;
        }
        //river
        else if (lanes[i][0] == -1){
            //opposite direction to previous if it exists
            if (lanes[i-1][0] == -1) lanes[i][1] = abs(lanes[i-1][1] - 1) ;  
            else lanes[i][1] = rand() % 2;
            
            //lane speed
            lanes[i][2] = (rand() % 30) + 15;
        }
    }         
}

void spawn_car(int lanes[ROWS][3], int grid[ROWS][COLS], int * cars_num, EntityCar cars[]){
    //select random row
    int row = (rand() % (ROWS - 1)) + 1;

    if (lanes[row][0] == 1){
        int col = (lanes[row][1]) ? 0 : COLS - 1;

        //if spawn place is empty
        if (grid[row][col] == 0){

            //if something spawned on this row lately, dont spawn
            int second = (col == 0) ? col + 1 : col - 1;
            int third = (col == 0) ? col + 2 : col - 2;

            if (grid[row][second] != 1 && grid[row][third] != 1) {
                int values[6] = {row, col, lanes[row][1], lanes[row][2], 1, 1};
                init_car(&cars[*cars_num], values);
                cars[*cars_num].type = car_type();

                *cars_num += 1;
            }
        }
    }
}

int car_type(){
    int type = (rand() % 20 == 0) ? 1 : 0; // friendly
    type = (rand() % 20 == 0) ? 2 : type; // taxi

    return type;
}