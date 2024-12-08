//custom headers
#include "cars.h"
#include "entities.h"
#include "config.h"

void remove_car(EntityCar * car){
    car->row = 0;
    car->col = 0;
    car->direction = 0;
    car->speed = 0;
    car->counter = 0;
    car->exists = 0;
}

void init_car(EntityCar * car, int values[6]){
    car->row = values[0];
    car->col = values[1];
    car->direction = values[2];
    car->speed = values[3];
    car->counter = values[4];
    car->exists = values[5];
}

void handle_car_collision(EntityCar * car, int grid[ROWS][COLS], EntityPlayer * player){
    int potential_col = (car->direction) ? car->col + 1 : car->col - 1;

    // check if car wants to go out of bounds
    if (potential_col == -1 || potential_col == COLS){
        remove_car(car);
    }

    switch (grid[car->row][potential_col])
    {
        case 3:     //frog
            car->col = potential_col;
            player->exists = 0;
            break;
        case 1:     //another car
            //dont move
            break; 
        default:    //nothing, so car moves
            car->col = potential_col;
            break;
    }

    
}

void move_car(EntityCar * car, int grid[ROWS][COLS], EntityPlayer * player){
    if (car->exists){
        //removes car from grid
        grid[car->row][car->col] = 0;

        //check car collision
        handle_car_collision(car, grid, player);
    }

    //if car still exists move it on grid
    if (car->exists) grid[car->row][car->col] = 1;
}

void compact_cars_array(EntityCar cars[MAX_CARS], int * cars_num) {
    int write_index = 0;

    for (int read_index = 0; read_index < *cars_num; read_index++) {
        if (cars[read_index].exists) {
            if (write_index != read_index) {
                cars[write_index] = cars[read_index]; //move valid car forward
            }
            write_index++;
        }
    }

    *cars_num = write_index; //update number of cars
}

void clear_cars_array(EntityCar cars[MAX_CARS], int *cars_num){
    for (int i = 0; i < MAX_CARS; i++)
    {
        remove_car(&cars[i]);
    }

    *cars_num = 0;
}
