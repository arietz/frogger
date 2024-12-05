#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define LANES 20

int main(){
    srand(time(NULL));
    int lane_types[20];
    //0 - forest, 1 - road, 2 - water, 10 - finish
    lane_types[0] = 0;
    lane_types[19] = 10;

    int combo = 1;

    for(int i = 1; i < 19; i++){
        int random = rand() % 16;
        
        //new combo
        if(combo == 1){
            lane_types[i] = rand() % 3;
        }
        //check combo
        else if(rand() % 16 > pow(2,combo) && lane_types[i-1] != 0){
            lane_types[i] = lane_types[i-1];
            combo++;
        }
        else {
            int temp = rand() % 5;
            if(temp == 0) lane_types[i] = 0;
            else if(temp == 1 || temp == 2) lane_types[i] = 1;
            else lane_types[i] = 2;
            
            combo = 1;
        }

    }

    for(int i = 19; i >= 0; i--){
        
        printf("%d \n", lane_types[i]);
    }
}