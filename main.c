#include <ncurses.h>
#include "map_generation.h"

#define ROWS 10
#define COLS 20

void initialize_ncurses() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
}

void cleanup_ncurses() {
    endwin();
}

void render_grid(int player_row, int player_col, int grid[ROWS][COLS]) {
    clear();
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            if (grid[i][j] == 1) mvprintw(i, j, "T");
        }
    }
    mvprintw(player_row, player_col, "F");
    refresh();
}

int main() {
//     int player_row = ROWS / 2;
//     int player_col = COLS / 2;
//     int ch;
//     int grid[ROWS][COLS] = {0};
//     grid[3][4] = 1;
//     grid[5][6] = 1;

//     initialize_ncurses();

//     while (1) {
//         render_grid(player_row, player_col, grid);
//         ch = getch();
//         switch (ch) {
//             case KEY_UP:
//                 if (player_row > 0 && grid[player_row - 1][player_col] == 0) player_row--;
//                 break;
//             case KEY_DOWN:
//                 if (player_row < ROWS - 1 && grid[player_row + 1][player_col] == 0) player_row++;
//                 break;
//             case KEY_LEFT:
//                 if (player_col > 0 && grid[player_row][player_col - 1] == 0) player_col--;
//                 break;
//             case KEY_RIGHT:
//                 if (player_col < COLS - 1 && grid[player_row][player_col + 1] == 0) player_col++;
//                 break;
//             case 'q':
//                 cleanup_ncurses();
//                 return 0;
//             default:
//                 break;
//         }
//     }

//     cleanup_ncurses();
    generate_map();
    return 0;
}
