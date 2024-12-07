#include <ncurses.h>
#include <sys/time.h> // For gettimeofday()
#include <stdlib.h> // For rand()

//custom header files
#include "map_generation.h"
#include "rendering.h"
#include "config.h"
#include "entities.h"


// Get the current time in milliseconds
long current_time_ms() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

void set_movement_delays(long * movement_opportunity, long last_movement[4]){
    long now = current_time_ms();

    movement_opportunity[0] = now - last_movement[0];
    movement_opportunity[1] = now - last_movement[1];
    movement_opportunity[2] = now - last_movement[2];
    movement_opportunity[3] = now - last_movement[3];
}

void move_car(EntityCar * car, int grid[ROWS][COLS]){
    if (car->direction) car->col++;
    else car->col--;

    if (car->col == 0 || car->col == COLS - 1){
        car->exists = 0;
    }

    //todo handling out of bound cars
}

int col_inbounds_check(int col, int change) {
    int new_col = col + change;
    if (new_col >= 0 && new_col < COLS) { // Logical AND for valid range
        return new_col;
    }
    return col; // Return current position if out of bounds
}

int row_inbounds_check(int row, int change) {
    int new_row = row + change;
    if (new_row >= 0 && new_row < ROWS) { // Ensure within grid bounds
        return new_row;
    }
    return row; // Return current position if out of bounds
}



void handle_player_collisions(EntityPlayer *player, int grid[ROWS][COLS], int row_move, int col_move) {
    int potential_row = row_inbounds_check(player->row, row_move);
    int potential_col = col_inbounds_check(player->col, col_move);

    // Handle collisions based on grid contents
    switch (grid[potential_row][potential_col]) {
        case -1: // Water
            // End game logic (placeholder)
            break;
        case 0: // Empty space
            grid[player->row][player->col] = 0; // Clear old position
            player->row = potential_row;
            player->col = potential_col;
            grid[player->row][player->col] = 3; // Mark new position
            break;
        case 1: // Blockades
            // Stay in place (do nothing)
            break;
        case 2: // Transport
            // Transport logic (placeholder)
            break;
        default:
            break;
    }
}

void handle_movement(EntityPlayer * player, int grid[ROWS][COLS], int key){
    int row_move = 0;
    int col_move = 0;
    
    switch (key) {
        case KEY_LEFT:
            col_move = -1;
            break;
        case KEY_RIGHT:
            col_move = 1;
            break;
        case KEY_UP:
            row_move = -1;
            break;
        case KEY_DOWN:
            row_move = 1;
            break;
        case 'q':
            endwin();
            exit(0);
        default:
            break;
    }

    //if changes are to be made check collisions
    if(row_move || col_move) handle_player_collisions(player, grid, row_move, col_move);
}

void map_reset(int grid[ROWS][COLS], EntityPlayer * player){
    //set player position
    player->col = COLS / 2;
    player->row = ROWS - 1;

    
}

int main() {
    initialize_ncurses();

    int lanes[ROWS] = {0};
    /* lanes logic for element values
    0   -> forest, contains blockades
    1   -> road, contains different types of cars
    2   -> river, contains boats
    10  -> finish line, completes level
    */
   
    int grid[ROWS][COLS] = {0};
    /* grid logic for element values
    -1  -> water, frog will drown in water
    0   -> empty space, safe for frog
    1   -> blockades, such as cars, trees etc.
    2   -> transport, such as boat or taxi
    3   -> frog 
    4
    5   -> frog on transport vehicle
    */

    //entity creation    
    EntityPlayer player;
    EntityCar cars[MAX_CARS] = {0};
    int cars_num = 0;

    map_reset(grid, &player);

    //movement delay for [0] - player, [1] - cars, [2] - boats, [3] - cars
    long movement_delays[4] = {0};
    long last_movement[4] = {0};

    int key;

    while (1) {
        // Render the grid and player
        render_grid(player, cars, cars_num, grid);
        
        set_movement_delays(movement_delays, last_movement);

        key = getch();

        // Process input for player movement
        if (movement_delays[0] >= PLAYER_DELAY) {
            handle_movement(&player, grid, key);

            // Reset movement timer only if a valid key was processed
            if (key == KEY_LEFT || key == KEY_RIGHT || key == KEY_UP || key == KEY_DOWN) {
                last_movement[0] = current_time_ms();
            }
        }

        //process car movement
        if (movement_delays[1] >= CAR_DELAY){
            for(int i = 0; i < cars_num; i++){
                if(cars[i].exists && (cars[i].counter >= cars[i].speed)){
                    move_car(&cars[i], grid);
                    cars[i].counter = 1;
                }
                else cars[i].counter ++;
            }
        }


        napms(10); // Small delay to prevent busy-waiting
    }

    endwin();
    return 0;
}