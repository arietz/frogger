#include <time.h>
#include <stdlib.h>
#include <math.h>

//custom headers
#include "map_generation.h"
#include "config.h"
#include "cars.h"

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

void generate_cars(unsigned int seed, int lanes[ROWS][3], int grid[ROWS][COLS]){
    //if given seed, use it
    srand((seed != 0) ? seed : time(NULL));

    for(int i = 0; i < ROWS; i++){
        if (lanes[i][0] != 1) continue;

        for(int j = 0; j < COLS; j++){
            if (rand() % 20 <= 5) grid[i][j] = 1; 
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
}
