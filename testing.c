// #include <ncurses.h>
// #include <stdbool.h>
// #include <stdlib.h>

// #define GRID_SIZE 10
// #define PLAYER_CHAR '@'
// #define EMPTY_CHAR '.'

// // Struct for player position
// typedef struct {
//     int x, y;
// } Player;

// // Function prototypes
// void initialize_ncurses();
// void cleanup_ncurses();
// void render_grid(Player player, Player previous_player);
// void handle_input(Player *player, Player *previous_player);

// int main() {
//     Player player = {GRID_SIZE / 2, GRID_SIZE / 2}; // Initial player position
//     Player previous_player = player;               // Track previous position
//     bool running = true;

//     // Initialize ncurses
//     initialize_ncurses();

//     first_render(player);

//     while (running) {
//         handle_input(&player, &previous_player); // Handle input
//         render_grid(player, previous_player);   // Render grid only if needed
//     }

//     cleanup_ncurses();
//     return 0;
// }

// void initialize_ncurses() {
//     initscr();
//     cbreak();
//     noecho();
//     keypad(stdscr, TRUE); // Enable special keys (e.g., arrows)
//     curs_set(0);          // Hide the cursor
//     nodelay(stdscr, TRUE); // Make getch() non-blocking
// }

// // Render the grid and the player
// void first_render(Player player) {
//     clear();
//     for (int row = 0; row < GRID_SIZE; row++) {
//         for (int col = 0; col < GRID_SIZE; col++) {
//             if (row == player.y && col == player.x) {
//                 mvprintw(row, col * 2, "%c", PLAYER_CHAR); // Player character
//             } else {
//                 mvprintw(row, col * 2, "%c", EMPTY_CHAR);  // Empty cell
//             }
//         }
//     }
//     mvprintw(GRID_SIZE, 0, "Use arrow keys to move. Press 'q' to quit.");
//     refresh();
// }

// void cleanup_ncurses() {
//     endwin();
// }

// void render_grid(Player player, Player previous_player) {
//     // Update only the cells that change
//     if (previous_player.x != player.x || previous_player.y != player.y) {
//         mvprintw(previous_player.y, previous_player.x * 2, "%c", EMPTY_CHAR); // Clear old position
//         mvprintw(player.y, player.x * 2, "%c", PLAYER_CHAR);                  // Draw new position
//         refresh();
//     }
// }

// void handle_input(Player *player, Player *previous_player) {
//     int ch = getch(); // Get input (non-blocking)

//     // Save the current position as previous
//     *previous_player = *player;

//     switch (ch) {
//         case KEY_UP:
//             if (player->y > 0) player->y--;
//             break;
//         case KEY_DOWN:
//             if (player->y < GRID_SIZE - 1) player->y++;
//             break;
//         case KEY_LEFT:
//             if (player->x > 0) player->x--;
//             break;
//         case KEY_RIGHT:
//             if (player->x < GRID_SIZE - 1) player->x++;
//             break;
//         case 'q': // Quit the game
//             cleanup_ncurses();
//             exit(0);
//     }
// }


#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
// #include "map_generation.h"

#define ROWS 15

int main(){
    unsigned int seed;
    scanf("%d", &seed);

    srand((seed != 0) ? seed : time(NULL));
    
    int lane_types[ROWS];

    //0 - forest, 1 - road, 2 - water, 10 - finish
    lane_types[0] = 0;
    lane_types[ROWS-1] = 10;

    int combo = 1;

    for(int i = 1; i < ROWS - 1; i++){        
        //new combo
        if(combo == 1){
            int temp = rand() % 6;

            if(temp <= 2) lane_types[i] = 1;
            else if(temp == 3) lane_types[i] = 0;
            else lane_types[i] = 2;
        }
        //check combo
        else if(rand() % 32 > pow(2,combo) && lane_types[i-1] != 0){
            lane_types[i] = lane_types[i-1];
            combo++;
        }
        else {
            lane_types[i] = 0;
            // int temp = rand() % 5;
            // if(temp == 0) lane_types[i] = 0;
            // else if(temp == 1 || temp == 2) lane_types[i] = 1;
            // else lane_types[i] = 2;
            
            combo = 1;
        }

    }

    for(int i = ROWS - 1; i >= 0; i--){
        printf("%d \n", lane_types[i]);
    }
}
