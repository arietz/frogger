
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

void handle_car_collision(EntityCar * car, int grid[ROWS][COLS]){
    int potential_col = (car->direction) ? car->col + 1 : car->col - 1;

    switch (grid[car->row][potential_col])
    {
        case 3:     //frog
            //lose
            break;
        case 1:     //another car
            //dont move
            break; 
        default:    //nothing, so car moves
            car->col = potential_col;
            break;
    }
}

void move_car(EntityCar * car, int grid[ROWS][COLS]){
    //removes car from grid
    grid[car->row][car->col] = 0;

    //check car collision
    handle_car_collision(car, grid);

    //if car goes out of bounds, remove it
    if (car->col == -1 || car->col == COLS) remove_car(car);
    //else add car to the grid
    else grid[car->row][car->col] = 1;
}