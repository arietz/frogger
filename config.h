#ifndef CONFIG_H
#define CONFIG_H

typedef struct {
    // gameplay configuration
    int ROWS;
    int COLS;
    int PLAYER_DELAY;
    int CAR_DELAY;
    int STORK_DELAY;
    int MAX_CARS;
    int RIVERS;
    int SEED;
    int RENDER_MODE_2X2;
    int TREE_CHANCE;

    // 1x1 character configurations
    char SYMBOL_WATER;
    char SYMBOL_FOREST;
    char SYMBOL_FINISH;
    char SYMBOL_TREE;
    char SYMBOL_CAR;
    char SYMBOL_FRIENDLY_CAR;
    char SYMBOL_TAXI;
    char SYMBOL_PLAYER;
    char SYMBOL_STORK;

    // 2x2 character configurations
    char SYMBOL_WATER_2X2[2][2];
    char SYMBOL_FOREST_2X2[2][2];
    char SYMBOL_FINISH_2X2[2][2];
    char SYMBOL_TREE_2X2[2][2];
    char SYMBOL_CAR_2X2[2][2];
    char SYMBOL_FRIENDLY_CAR_2X2[2][2];
    char SYMBOL_TAXI_2X2[2][2];
    char SYMBOL_PLAYER_2X2[2][2];
    char SYMBOL_STORK_2X2[2][2];

    int COLOR_ROAD;
    int COLOR_WATER;
    int COLOR_FOREST;
    int COLOR_FINISH;
    int COLOR_TREE;
    int COLOR_CAR;
    int COLOR_FRIENDLY_CAR;
    int COLOR_PLAYER;
    int COLOR_STORK;
} Config;

void load_config();

#endif // CONFIG_H
