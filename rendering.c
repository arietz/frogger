#include <ncurses.h>
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

void render_grid(EntityPlayer player, EntityCar cars[], int cars_num, int grid[ROWS][COLS]) {
    clear();

    // Draw top border
    for (int j = 0; j <= COLS + 1; ++j) {
        mvprintw(0, j, "-"); // Top border
    }

    // Draw the grid with left and right borders
    for (int i = 0; i < ROWS; ++i) {
        mvprintw(i + 1, 0, "|"); // Left border
        for (int j = 0; j < COLS; ++j) {
            if (grid[i][j] == 1)
                mvprintw(i + 1, j + 1, "#"); // Block in grid
        }
        mvprintw(i + 1, COLS + 1, "|"); // Right border
    }

    // Draw bottom border
    for (int j = 0; j <= COLS + 1; ++j) {
        mvprintw(ROWS + 1, j, "-"); // Bottom border
    }

    // Draw the player
    mvprintw(player.row + 1, player.col + 1, "F");

    // Draw the cars
    for (int i = 0; i < cars_num; i++) {
        mvprintw(cars[i].row + 1, cars[i].col + 1, "D");
    }

    refresh();
}
