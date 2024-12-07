#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

//custom headers
#include "map_generation.h"
#include "config.h"

void generate_lanes(unsigned int seed, int lanes[ROWS]){
    //if given seed, use it
    srand((seed != 0) ? seed : time(NULL));

    //static values of first and last lane
    lanes[0] = 10;      //finish 
    lanes[ROWS-1] = 0;  //start

    //combo system for higher chance of the same lane order
    int combo = 1;

    for(int i = 1; i < ROWS - 1; i++){  

        //new combo
        if(combo == 1){
            int temp = rand() % 6;

            //50% chance for road, 33% chance for river, 17% chance for forest
            if(temp <= 2) lanes[i] = 1;         //road
            else if(temp == 3) lanes[i] = 0;    //forest
            else lanes[i] = -1;                 //river
        }
        //check if combo persists and it isn't forest combo
        else if(rand() % 32 > pow(2,combo) && lanes[i-1] != 0){
            lanes[i] = lanes[i-1];
            combo++;
        }
        //combo is broken, for gameplay's sake we set lane to forest
        else {
            lanes[i] = 0;
            combo = 1;
        }
    }
}
