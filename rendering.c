#include <ncurses.h>
#include <locale.h>
#include <string.h>

// Custom headers
#include "entities.h"
#include "config.h"

void initialize_ncurses() {
    setlocale(LC_ALL, "");
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    nodelay(stdscr, TRUE);
}

char get_lane_symbol(int type, Config * cfg) {
    switch (type) {
        case -1:
            return cfg->SYMBOL_WATER;
        case 0:
            return cfg->SYMBOL_FOREST;
        case 10:
            return cfg->SYMBOL_FINISH;
        default:
            return ' ';
    }
}

void render_2x2(int row, int col, char sprite[2][2]){
    // Render the 2x2 sprite
    mvprintw(2 * row + 1, 2 * col + 1, "%c%c", sprite[0][0], sprite[0][1]);
    mvprintw(2 * row + 2, 2 * col + 1, "%c%c", sprite[1][0], sprite[1][1]);
}

void render_cell(int row, int col, char symbol, Config *cfg) {
    if (cfg->RENDER_MODE_2X2) {
        // Select 2x2 sprite based on the symbola
        if (symbol == cfg->SYMBOL_WATER) render_2x2(row, col, cfg->SYMBOL_WATER_2X2);
        else if (symbol == cfg->SYMBOL_FOREST) render_2x2(row, col, cfg->SYMBOL_FOREST_2X2);
        else if (symbol == cfg->SYMBOL_FINISH) render_2x2(row, col, cfg->SYMBOL_FINISH_2X2);
        else if (symbol == cfg->SYMBOL_TREE) render_2x2(row, col, cfg->SYMBOL_TREE_2X2);
        else if (symbol == cfg->SYMBOL_CAR) render_2x2(row, col, cfg->SYMBOL_CAR_2X2);
        else if (symbol == cfg->SYMBOL_FRIENDLY_CAR) render_2x2(row, col, cfg->SYMBOL_FRIENDLY_CAR_2X2);
        else if (symbol == cfg->SYMBOL_TAXI) render_2x2(row, col, cfg->SYMBOL_TAXI_2X2);
        else if (symbol == cfg->SYMBOL_PLAYER) render_2x2(row, col, cfg->SYMBOL_PLAYER_2X2);
        else {
            char sprite[2][2] = {{' ',' '},{' ',' '}};
            render_2x2(row, col, sprite);
        }  
    } else {
        // Render in 1x1 block mode
        mvprintw(row + 1, col + 1, "%c", symbol);
    }
}


void clear_border(int ** grid, int ** lanes, Config * cfg){
    for (int i = 0; i < cfg->ROWS; i++) {
        char symbol = get_lane_symbol(lanes[i][0], cfg);
        int temp = (lanes[i][0] == 1) ? 0 : lanes[i][0];
        if (grid[i][0] == temp) render_cell(i, 0, symbol, cfg);
        if (grid[i][cfg->COLS - 1] == temp) render_cell(i, cfg->COLS - 1, symbol, cfg);
    } 
}

void render_border_top(Config * cfg){
    mvprintw(0, 0, "┌");
    for (int j = 1; j <= (cfg->RENDER_MODE_2X2 ? 2 * cfg->COLS : cfg->COLS); ++j) {
        mvprintw(0, j, "─");
    }
    mvprintw(0, (cfg->RENDER_MODE_2X2 ? 2 * cfg->COLS : cfg->COLS) + 1, "┐");
}

void render_border_sides(int row, int col, Config * cfg){
    if (cfg->RENDER_MODE_2X2) {
        mvprintw(2 * row, col ? 2 * col - 1 : 2 * col, "│");
        mvprintw(2 * row - 1, col ? 2 * col - 1 : 2 * col, "│");
    }
    else mvprintw(row, col, "│");
}   

void render_border_bottom(Config * cfg){
    mvprintw(cfg->RENDER_MODE_2X2 ? 2 * cfg->ROWS + 1 : cfg->ROWS + 1, 0, "└");
    for (int j = 1; j <= (cfg->RENDER_MODE_2X2 ? 2 * cfg->COLS : cfg->COLS); ++j) {
        mvprintw(cfg->RENDER_MODE_2X2 ? 2 * cfg->ROWS + 1 : cfg->ROWS + 1, j, "─");
    }
    mvprintw(cfg->RENDER_MODE_2X2 ? 2 * cfg->ROWS + 1 : cfg->ROWS + 1, (cfg->RENDER_MODE_2X2 ? 2 * cfg->COLS : cfg->COLS) + 1, "┘");
}

void render_map(int ** grid, int ** lanes, Config * cfg) {
    clear();

    // Top border
    render_border_top(cfg);

    // Playable space with borders
    for (int i = 0; i < cfg->ROWS; ++i) {
        //left border
        render_border_sides(i + 1, 0, cfg); 

        for (int j = 0; j < cfg->COLS; ++j) {
            // Lane default
            render_cell(i, j, get_lane_symbol(lanes[i][0], cfg), cfg);

            // Tree/blockade
            if (grid[i][j] == 1) {
                render_cell(i, j, cfg->SYMBOL_TREE, cfg);
            }
        }

        //right border
        render_border_sides(i + 1, cfg->COLS + 1, cfg);
    }

    // Bottom border
    render_border_bottom(cfg);

    refresh();
}

void render_entities(int ** grid, int ** lanes, EntityPlayer *player, EntityCar * cars, int *cars_num, Config * cfg) {
    // Draw the player
    if (player->exists) {
        char symbol = get_lane_symbol(lanes[player->prev_row][0], cfg);
        render_cell(player->prev_row, player->prev_col, symbol, cfg);
        render_cell(player->row, player->col, cfg->SYMBOL_PLAYER, cfg);
    }

    // Draw the cars
    for (int i = 0; i < *cars_num; i++) {
        if (cars[i].exists) {
            // Clear the car's previous position
            char lane_symbol = get_lane_symbol(lanes[cars[i].row][0], cfg);
            int difference = (cars[i].direction) ? 1 : -1;
            int prev_col = cars[i].col - difference;

            // Check if the previous column is within bounds
            if (prev_col >= 0 && prev_col < cfg->COLS) {
                render_cell(cars[i].row, prev_col, lane_symbol, cfg);
            }

            // select the symbol for car
            char symbol = cfg->SYMBOL_CAR;
            if (cars[i].type == 1) symbol = cfg->SYMBOL_FRIENDLY_CAR;
            else if (cars[i].type == 2) symbol = cfg->SYMBOL_TAXI;

            //render the car
            render_cell(cars[i].row, cars[i].col, symbol, cfg);
        }
    }

    clear_border(grid, lanes, cfg);

    // Display car count
    mvprintw(cfg->RENDER_MODE_2X2 ? 2 * cfg->ROWS + 3 : cfg->ROWS + 2, 3, "Cars: %d", *cars_num);

    refresh(); // Render all changes to the screen
}
