#include <ncurses.h>
#include <sys/time.h> // For gettimeofday()
#include <stdlib.h> // For rand()

//custom header files
#include "map_generation.h"
#include "rendering.h"
#include "cars.h"
#include "config.h"
#include "entities.h"
#include "player.h"


//gets current time in milliseconds
long current_time_ms() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

void set_movement_delays(long * movement_opportunity, long * last_movement){
    long now = current_time_ms();

    movement_opportunity[0] = now - last_movement[0];
    movement_opportunity[1] = now - last_movement[1];
    movement_opportunity[2] = now - last_movement[2];
}

void gameloop(EntityPlayer *player, EntityCar * cars, int **grid, int **lanes, int *cars_num, Config * cfg) {
    //movement delay for [0] - player, [1] - cars, [2] - stork
    long movement_delays[4] = {0};
    long last_movement[4] = {0};
    int spawn_counter = 0;

    int key;
    render_map(grid, lanes, cfg);

    //game loop for map
    while (!player->finished && player->exists) {
        set_movement_delays(movement_delays, last_movement);

        key = getch();

        //process input for player movement
        if (movement_delays[0] >= cfg->PLAYER_DELAY) {
            handle_player_movement(player, grid, key, cfg);

            //reset movement timer only if a valid key was processed
            if (key == KEY_LEFT || key == KEY_RIGHT || key == KEY_UP || key == KEY_DOWN) {
                last_movement[0] = current_time_ms();
            }
        }

        // Process car movement
        if (movement_delays[1] >= cfg->CAR_DELAY) {
            for (int i = 0; i < *cars_num; i++) {
                if (cars[i].exists && (cars[i].counter >= cars[i].speed)) {
                    move_car(&cars[i], grid, lanes, player, cfg);
                    cars[i].counter = 1;
                } else {
                    cars[i].counter++;
                }
            }

            // Compact the cars array to remove unused slots
            compact_cars_array(cars, cars_num, cfg);

            // Spawn car
            if (spawn_counter == 3) {
                spawn_car(lanes, grid, cars_num, cars, cfg);
                spawn_counter = 0;
            } else {
                spawn_counter++;
            }

            last_movement[1] = current_time_ms();
        }

        render_entities(grid, lanes, player, cars, cars_num, cfg);

        napms(10); // Small delay
    }
}



int main() {
    initialize_ncurses();
    Config * cfg = malloc(sizeof(Config));
    load_config(cfg);

    // Dynamically allocate memory
    int **lanes = malloc(cfg->ROWS * sizeof(int *));
    for (int i = 0; i < cfg->ROWS; i++) {
        lanes[i] = calloc(3, sizeof(int));
    }

    int **grid = malloc(cfg->ROWS * sizeof(int *));
    for (int i = 0; i < cfg->ROWS; i++) {
        grid[i] = calloc(cfg->COLS, sizeof(int));
    }

    EntityCar *cars = malloc(cfg->MAX_CARS * sizeof(EntityCar));


    /* roads logic
    if in lanes array there is a road or rivers 
    we can find details about them here
    [0] -> type, forest = 0, road = 1, river = -1, finish = 10
    [1] -> direction, to right = 1, to left = 0
    [2] -> lane speed
    */

    /* grid logic for element values
    0   -> empty space, safe for frog
    -1  -> water, kills upon contact
    1   -> blockades, such as cars, trees etc.
    2   -> transport, such as boat or taxi
    3   -> frog 
    4
    5   -> frog on transport vehicle
    10  -> finish line
    */

   /*
    0   -> tree
    1   -> car
    2   -> friendly car
    3   -> taxi
    4   -> boat
    5   -> frog
    6   -> water
    7   -> road
    8   -> forest
    9   -> empty
   */

    //entity creation    
    EntityPlayer player = {0,0,1,0,0,0};

    int cars_num = 0;

    while (player.exists) {
        clear_cars_array(cars, &cars_num, cfg);
        map_reset(grid, lanes, &player, cars, &cars_num, cfg->SEED, cfg);
        gameloop(&player, cars, grid, lanes, &cars_num, cfg);
        player.finished = 0;
    }

    render_entities(grid, lanes, &player, cars, &cars_num, cfg);
    

    napms(100);

    // Free allocated memory
    for (int i = 0; i < cfg->ROWS; i++) {
        free(lanes[i]);
        free(grid[i]);
    }
    free(lanes);
    free(grid);
    free(cars);
    free(cfg);

    endwin();
    return 0;
}