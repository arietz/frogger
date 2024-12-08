#include <ncurses.h>
#include <locale.h>
#include <string.h>
#include<stdlib.h>

// Custom headers
#include "entities.h"
#include "config.h"

void initialize_ncurses() {
    setlocale(LC_ALL, "");
    initscr();
    if (has_colors() == FALSE) {
        endwin();
        fprintf(stderr, "Your terminal does not support color\n");
        exit(1);
    }
    start_color();
    use_default_colors();

    //lane colors
    init_pair(1, COLOR_BLACK, COLOR_BLACK);     //empty space
    init_pair(2, COLOR_CYAN, COLOR_BLUE);       //water
    init_pair(3, COLOR_GREEN, COLOR_BLACK);     //forest
    init_pair(20, COLOR_WHITE, COLOR_WHITE);    //finish1
    init_pair(21, COLOR_BLACK, COLOR_BLACK);    //finish2

    //object colors
    init_pair(4, COLOR_GREEN, COLOR_BLACK);     //tree
    init_pair(5, COLOR_RED, COLOR_BLACK);       //car
    init_pair(6, COLOR_MAGENTA, COLOR_BLACK);   //friendly car
    init_pair(10, COLOR_WHITE, COLOR_BLACK);    //player
    init_pair(11, COLOR_BLUE, COLOR_BLACK);     //taxi
    init_pair(12, COLOR_WHITE, COLOR_BLACK);    //taxi with player
    init_pair(13, COLOR_YELLOW, COLOR_BLUE);    //boat
    init_pair(14, COLOR_WHITE, COLOR_BLUE);     //boat with player
    init_pair(20, COLOR_BLACK, COLOR_WHITE);    //stork
    
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
    //finish rendering
    if (row == 0) {
        attron(COLOR_PAIR(20));
        mvprintw(2 * row + 1, 2 * col + 1, "%c", sprite[0][0]);
        mvprintw(2 * row + 2, 2 * col + 2, "%c", sprite[1][1]);
        attroff(COLOR_PAIR(20));
        attron(COLOR_PAIR(21));
        mvprintw(2 * row + 1, 2 * col + 2, "%c", sprite[0][1]);
        mvprintw(2 * row + 2, 2 * col + 1, "%c", sprite[1][0]);
        attroff(COLOR_PAIR(21));
    }
    else{
        mvprintw(2 * row + 1, 2 * col + 1, "%c%c", sprite[0][0], sprite[0][1]);
        mvprintw(2 * row + 2, 2 * col + 1, "%c%c", sprite[1][0], sprite[1][1]);
    }    
}

int get_color(int row, int col, char symbol, Config *cfg, int ** grid, int ** lanes){
    int color_pair = 1;

    // Determine color based on symbol
    if (symbol == cfg->SYMBOL_WATER)                color_pair = cfg->COLOR_WATER;
    else if (symbol == cfg->SYMBOL_FOREST)          color_pair = cfg->COLOR_FOREST;
    else if (symbol == cfg->SYMBOL_FINISH)          color_pair = cfg->COLOR_FINISH;
    else if (symbol == cfg->SYMBOL_TREE)            color_pair = cfg->COLOR_TREE;
    else if (symbol == cfg->SYMBOL_CAR)             color_pair = cfg->COLOR_CAR;
    else if (symbol == cfg->SYMBOL_FRIENDLY_CAR)    color_pair = cfg->COLOR_FRIENDLY_CAR;
    else if (symbol == cfg->SYMBOL_PLAYER)          color_pair = cfg->COLOR_PLAYER;
    else if (symbol == cfg->SYMBOL_STORK)           color_pair = cfg->COLOR_STORK;
    else if (symbol == cfg->SYMBOL_TAXI){
        //if its taxi
        if(lanes[row][0] == 1){
            if(grid[row][col] > 3) color_pair = 12;
            else color_pair = 11;
        }
        //if its boat
        else{
            if(grid[row][col] > 3) color_pair = 14;
            else color_pair = 13;
        }
    }

    return color_pair;
}

void select_render(int row, int col, char symbol, Config *cfg, int ** grid, int ** lanes){
    if (symbol == cfg->SYMBOL_WATER) render_2x2(row, col, cfg->SYMBOL_WATER_2X2);
    else if (symbol == cfg->SYMBOL_FOREST) render_2x2(row, col, cfg->SYMBOL_FOREST_2X2);
    else if (symbol == cfg->SYMBOL_FINISH) render_2x2(row, col, cfg->SYMBOL_FINISH_2X2);
    else if (symbol == cfg->SYMBOL_TREE) render_2x2(row, col, cfg->SYMBOL_TREE_2X2);
    else if (symbol == cfg->SYMBOL_CAR) render_2x2(row, col, cfg->SYMBOL_CAR_2X2);
    else if (symbol == cfg->SYMBOL_FRIENDLY_CAR) render_2x2(row, col, cfg->SYMBOL_FRIENDLY_CAR_2X2);
    else if (symbol == cfg->SYMBOL_TAXI) render_2x2(row, col, cfg->SYMBOL_TAXI_2X2);
    else if (symbol == cfg->SYMBOL_PLAYER) render_2x2(row, col, cfg->SYMBOL_PLAYER_2X2);
    else if (symbol == cfg->SYMBOL_STORK) render_2x2(row, col, cfg->SYMBOL_STORK_2X2);
    else {
        //empty space
        char sprite[2][2] = {{' ',' '},{' ',' '}};
        render_2x2(row, col, sprite);
    }  
}

void render_cell(int row, int col, char symbol, Config *cfg, int ** grid, int ** lanes) {
    
    //turn on color
    int color_pair = get_color(row, col, symbol, cfg, grid, lanes);
    attron(COLOR_PAIR(color_pair));

    if (cfg->RENDER_MODE_2X2) {
        //2x2 mode
        select_render(row, col, symbol, cfg, grid, lanes);
        
    } else {
        //1x1 mode
        if (row == 0 && col % 2 == 1){
            attroff(COLOR_PAIR(color_pair));
            attron(COLOR_PAIR(21));
            mvprintw(row + 1, col + 1, "%c", symbol);
            attroff(COLOR_PAIR(21));
        }
        else mvprintw(row + 1, col + 1, "%c", symbol);
    }

    //turn off color
    attroff(COLOR_PAIR(color_pair));
    
}

void clear_border(int ** grid, int ** lanes, Config * cfg){
    for (int i = 0; i < cfg->ROWS; i++) {
        char symbol = get_lane_symbol(lanes[i][0], cfg);
        int temp = (lanes[i][0] == 1) ? 0 : lanes[i][0];
        
        if (grid[i][0] == temp) render_cell(i, 0, symbol, cfg, grid, lanes);
        if (grid[i][cfg->COLS - 1] == temp) render_cell(i, cfg->COLS - 1, symbol, cfg, grid, lanes);
    } 
}

void render_border_top(Config * cfg){
    attron(COLOR_PAIR(cfg->COLOR_FOREST)); // Assuming top border is forest
    mvprintw(0, 0, "┌");
    for (int j = 1; j <= (cfg->RENDER_MODE_2X2 ? 2 * cfg->COLS : cfg->COLS); ++j) {
        mvprintw(0, j, "─");
    }
    mvprintw(0, (cfg->RENDER_MODE_2X2 ? 2 * cfg->COLS : cfg->COLS) + 1, "┐");
    attroff(COLOR_PAIR(cfg->COLOR_FOREST));
}

void render_border_sides(int row, int col, Config * cfg){
    attron(COLOR_PAIR(cfg->COLOR_FOREST)); // Assuming side borders are forest
    if (cfg->RENDER_MODE_2X2) {
        mvprintw(2 * row, col ? 2 * col - 1 : 2 * col, "│");
        mvprintw(2 * row - 1, col ? 2 * col - 1 : 2 * col, "│");
    }
    else {
        mvprintw(row, col, "│");
    }
    attroff(COLOR_PAIR(cfg->COLOR_FOREST));
}

void render_border_bottom(Config * cfg){
    attron(COLOR_PAIR(cfg->COLOR_FOREST)); // Assuming bottom border is forest
    mvprintw(cfg->RENDER_MODE_2X2 ? 2 * cfg->ROWS + 1 : cfg->ROWS + 1, 0, "└");
    for (int j = 1; j <= (cfg->RENDER_MODE_2X2 ? 2 * cfg->COLS : cfg->COLS); ++j) {
        mvprintw(cfg->RENDER_MODE_2X2 ? 2 * cfg->ROWS + 1 : cfg->ROWS + 1, j, "─");
    }
    mvprintw(cfg->RENDER_MODE_2X2 ? 2 * cfg->ROWS + 1 : cfg->ROWS + 1, 
             (cfg->RENDER_MODE_2X2 ? 2 * cfg->COLS : cfg->COLS) + 1, "┘");
    attroff(COLOR_PAIR(cfg->COLOR_FOREST));
}

void render_map(int ** grid, int ** lanes, Config * cfg, int * level, int * points) {
    clear();

    // Top border
    render_border_top(cfg);

    // Playable space with borders
    for (int i = 0; i < cfg->ROWS; ++i) {
        //left border
        render_border_sides(i + 1, 0, cfg); 

        for (int j = 0; j < cfg->COLS; ++j) {
            //lane
            render_cell(i, j, get_lane_symbol(lanes[i][0], cfg), cfg, grid, lanes);

            //tree
            if (grid[i][j] == 1) {
                render_cell(i, j, cfg->SYMBOL_TREE, cfg, grid, lanes);
            }
        }

        //right border
        render_border_sides(i + 1, cfg->COLS + 1, cfg);
    }

    //bottom border
    render_border_bottom(cfg);

    //stats
    mvprintw(cfg->RENDER_MODE_2X2 ? cfg->ROWS * 2 + 2 : cfg->ROWS + 2, 2, "Level: %d  Points: %d", *level, *points);
    
    //author
    mvprintw(cfg->RENDER_MODE_2X2 ? cfg->ROWS * 2 + 3 : cfg->ROWS + 3, 2, "Author: Aleksander Rietz, ID: 203274");

    refresh();
}

char get_grid_symbol(int row, int col, int ** grid, int ** lanes, Config * cfg){
    int g = grid[row][col];
    int l = lanes[row][0];

    if (g == -1) return cfg->SYMBOL_WATER;
    else if (g == 0 && l == 0) return cfg->SYMBOL_FOREST;
    else if (g == 10) return cfg->SYMBOL_FINISH;
    else if (g == 1 && l == 0) return cfg->SYMBOL_TREE;
    else if (g == 1 && l == 1) return cfg->SYMBOL_CAR;
    else if (g == 1 && l == 2) return cfg->SYMBOL_TAXI;
    else return ' ';
}

void render_player(EntityPlayer *player, Config *cfg, int **grid, int **lanes) {
    if (player->exists) {
        char symbol = get_lane_symbol(lanes[player->prev_row][0], cfg);
        render_cell(player->prev_row, player->prev_col, symbol, cfg, grid, lanes);
        render_cell(player->row, player->col, cfg->SYMBOL_PLAYER, cfg, grid, lanes);
    } else {
        char symbol = get_lane_symbol(lanes[player->row][0], cfg);
        render_cell(player->row, player->col, symbol, cfg, grid, lanes);
    }
}

void render_cars(EntityCar *cars, int *cars_num, Config *cfg, int **grid, int **lanes) {
    for (int i = 0; i < *cars_num; i++) {
        if (cars[i].exists) {
            char lane_symbol = get_lane_symbol(lanes[cars[i].row][0], cfg);
            int difference = (cars[i].direction) ? 1 : -1;
            int prev_col = cars[i].col - difference;
            if ((prev_col >= 0 && prev_col < cfg->COLS) && (grid[cars[i].row][prev_col] == 0 || grid[cars[i].row][prev_col] == -1)) {
                render_cell(cars[i].row, prev_col, lane_symbol, cfg, grid, lanes);
            }
            char symbol = (cars[i].type == 1) ? cfg->SYMBOL_FRIENDLY_CAR :
                          (cars[i].type == 2 || cars[i].type == 3) ? cfg->SYMBOL_TAXI : cfg->SYMBOL_CAR;
            render_cell(cars[i].row, cars[i].col, symbol, cfg, grid, lanes);
        }
    }
}

void render_entities(int **grid, int **lanes, EntityPlayer *player, EntityCar *cars, int *cars_num, Config *cfg, EntityStork *stork) {
    render_player(player, cfg, grid, lanes);
    
    //remove previous stork
    if(stork->exists){
        char symbol = get_grid_symbol(stork->prev_row, stork->prev_col, grid, lanes, cfg);
        render_cell(stork->prev_row, stork->prev_col, symbol, cfg, grid, lanes);
    }

    render_cars(cars, cars_num, cfg, grid, lanes);
    
    clear_border(grid, lanes, cfg);

    //draw the stork
    if(stork->exists){
        render_cell(stork->row, stork->col, cfg->SYMBOL_STORK, cfg, grid, lanes);
    }
    refresh();
}

