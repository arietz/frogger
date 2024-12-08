#include <ncurses.h>

#include "config.h"
#include "entities.h"

int col_inbounds_check(int col, int change, Config * cfg) {
    int new_col = col + change;
    if (new_col >= 0 && new_col < cfg->COLS) { // Logical AND for valid range
        return new_col;
    }
    return col; // Return current position if out of bounds
}

int row_inbounds_check(int row, int change, Config * cfg) {
    int new_row = row + change;
    if (new_row >= 0 && new_row < cfg->ROWS) { // Ensure within grid bounds
        return new_row;
    }
    return row; // Return current position if out of bounds
}

void handle_player_collisions(EntityPlayer *player, int ** grid, int row_move, int col_move, Config * cfg) {
    int potential_row = row_inbounds_check(player->row, row_move, cfg);
    int potential_col = col_inbounds_check(player->col, col_move, cfg);
    player->prev_row = player->row;
    player->prev_col = player->col;

    // Handle collisions based on grid contents
    switch (grid[potential_row][potential_col]) {
        case -1:    //water
            player->exists = 0;

            break;
        case 10:    //finish line
            player->finished = 1;
        case 0:     //empty space
            grid[player->row][player->col] -= 3; // Clear old position
            player->row = potential_row;
            player->col = potential_col;
            grid[player->row][player->col] += 3; // Mark new position
            break;
        case 1:     //blockades
            // Stay in place (do nothing)
            break;
        case 2:     //transport
            grid[player->row][player->col] -= 3; // Clear old position
            player->row = potential_row;
            player->col = potential_col;
            grid[player->row][player->col] += 3; // Mark new position
            break;
        default:
            break;
    }
}

void handle_player_movement(EntityPlayer * player, int ** grid, int key, Config * cfg){
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
        default:
            break;
    }

    //if changes are to be made check collisions
    if(row_move || col_move) handle_player_collisions(player, grid, row_move, col_move, cfg);
}
