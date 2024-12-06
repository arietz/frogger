#include <ncurses.h>
#include <stdlib.h>

#define ROWS 20
#define COLS 11
#define MOVE_DELAY 100 // Movement delay in milliseconds

typedef struct {
    int x;
    int y;
} EntityPlayer;

void initialize_ncurses() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    nodelay(stdscr, TRUE); // Make getch() non-blocking
}

void render_grid(EntityPlayer player, int grid[ROWS][COLS]) {
    clear();
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            if (grid[i][j] == 1) mvprintw(i, j, "T");
        }
    }
    mvprintw(player.y, player.x, "F");
    refresh();
}

int main() {
    int ch;
    int grid[ROWS][COLS] = {0};
    EntityPlayer player = {19, 6};

    // Blockades
    grid[3][4] = 1;
    grid[5][6] = 1;

    initialize_ncurses();

    int gotchar = 0;

    while (1) {
        if (!gotchar) {
            ch = getch();
            gotchar = 1; // Mark that input has been fetched
        }

        render_grid(player, grid);

        // Process input
        switch (ch) {
            case KEY_LEFT:
                if (player.x > 0 && grid[player.y][player.x - 1] == 0) {
                    player.x--;
                }
                break;
            case KEY_RIGHT:
                if (player.x < COLS - 1 && grid[player.y][player.x + 1] == 0) {
                    player.x++;
                }
                break;
            case KEY_UP:
                if (player.y > 0 && grid[player.y - 1][player.x] == 0) {
                    player.y--;
                }
                break;
            case KEY_DOWN:
                if (player.y < ROWS - 1 && grid[player.y + 1][player.x] == 0) {
                    player.y++;
                }
                break;
            case 'q':
                endwin();
                exit(0);
            default:
                gotchar = 0; // Reset gotchar for unrecognized input
                break;
        }

        // Reset gotchar after processing the input
        gotchar = 0;

        napms(100); // Small delay to prevent busy-waiting
    }

    endwin();
    return 0;
}
