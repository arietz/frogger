#include <ncurses.h>

//custom headers
#include "entities.h"
#include "config.h"

void initialize_ncurses() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    nodelay(stdscr, TRUE); // Make getch() non-blocking
}

void render_map(EntityPlayer player, EntityCar cars[], int cars_num, int grid[ROWS][COLS], int lanes[ROWS][3]) {
    clear();

    //top border
    for (int j = 0; j <= COLS + 1; ++j) {
        mvprintw(0, j, "-");
    }

    //draw playable space with borders
    for (int i = 0; i < ROWS; ++i) {
        mvprintw(i + 1, 0, "|"); //left border

        //playable space
        for (int j = 0; j < COLS; ++j) {
            if (grid[i][j] == 1) mvprintw(i + 1, j + 1, "#");       //tree / blockade
            else if (lanes[i][0] == 0) mvprintw(i + 1, j + 1, ".");    //forest
            else if (lanes[i][0] == -1) mvprintw(i + 1, j + 1, "~");   //water
            else if (lanes[i][0] == 10) mvprintw(i + 1, j + 1, "=");   //finish
        }

        mvprintw(i + 1, COLS + 1, "|"); //right border
    }

    //bottom border
    for (int j = 0; j <= COLS + 1; ++j) {
        mvprintw(ROWS + 1, j, "-");
    }

    mvprintw(ROWS + 2, 3, "Cars: %d", cars_num);

    // Draw the player
    if (player.exists) mvprintw(player.row + 1, player.col + 1, "F");

    // Draw the cars
    for (int i = 0; i < cars_num; i++) {
        if (cars[i].exists) mvprintw(cars[i].row + 1, cars[i].col + 1, "D");
    }

    refresh();
}
