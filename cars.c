//custom headers
#include "cars.h"
#include "entities.h"
#include "config.h"
#include "player.h"

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

void handle_car_collisions(EntityCar * car, int ** grid, EntityPlayer * player, Config * cfg){
    int potential_col = (car->direction) ? car->col + 1 : car->col - 1;

    // check if car wants to go out of bounds
    if (potential_col == -1 || potential_col == cfg->COLS){
        if(car->type == 2 || car->type == 3){
            if (player->row == car->row && player->col == car->col){
                player->exists = 0;
            }
        }
        
        remove_car(car);
    }

    switch (grid[car->row][potential_col])
    {
        case 3:     //frog
            //exception for friendly cars
            if (car->type != 0) break;

            car->col = potential_col;
            player->exists = 0;
            break;
        case 2:
        case 1:     //another car
            //dont move
            break; 
        default:    //nothing, so car moves
            car->prev_col = car->col;
            car->col = potential_col;            
            break;
    }
}

void move_car(EntityCar * car, int ** grid, int ** lanes, EntityPlayer * player, Config * cfg){
    int lane_type = (lanes[car->row][0] == 1) ? 0 : lanes[car->row][0];
    // int temp = grid[car->row][car->col];

    if (car->exists){
        //removes car from grid
        grid[car->row][car->col] -= (car->type == 0) ? 1 : car->type;

        //check car collision
        handle_car_collisions(car, grid, player, cfg);
    }

    //if car still exists move it on grid
    if (car->exists) {
        grid[car->row][car->col] += (car->type == 0) ? 1 : car->type;
        if(car->type == 2 || car->type == 3){
            if (player->row == car->row && player->col == car->prev_col){
                handle_player_collisions(player, grid, 0, car->col-car->prev_col, cfg);
            }
        }
    }
}

void compact_cars_array(EntityCar * cars, int * cars_num) {
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

void clear_cars_array(EntityCar * cars, int *cars_num, Config * cfg){
    for (int i = 0; i < cfg->MAX_CARS; i++)
    {
        remove_car(&cars[i]);
    }

    *cars_num = 0;
}
