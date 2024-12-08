#ifndef CONFIG_H
#define CONFIG_H

typedef struct {
    // Gameplay configuration
    int ROWS;
    int COLS;
    int PLAYER_DELAY;
    int CAR_DELAY;
    int MAX_CARS;
    int SEED;
    int RENDER_MODE_2X2;

    // 1x1 character configurations
    char SYMBOL_WATER;
    char SYMBOL_FOREST;
    char SYMBOL_FINISH;
    char SYMBOL_TREE;
    char SYMBOL_CAR;
    char SYMBOL_FRIENDLY_CAR;
    char SYMBOL_TAXI;
    char SYMBOL_PLAYER;

    // 2x2 character configurations (as strings)
    char SYMBOL_WATER_2X2[2][2];
    char SYMBOL_FOREST_2X2[2][2];
    char SYMBOL_FINISH_2X2[2][2];
    char SYMBOL_TREE_2X2[2][2];
    char SYMBOL_CAR_2X2[2][2];
    char SYMBOL_FRIENDLY_CAR_2X2[2][2];
    char SYMBOL_TAXI_2X2[2][2];
    char SYMBOL_PLAYER_2X2[2][2];
} Config;

// Function prototypes
void load_config();

#endif // CONFIG_H
