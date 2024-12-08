#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"

void load_options(const char *filename, Config * cfg) {
    FILE *file = fopen(filename, "r");

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char key[64];
        char value[64];
        if (sscanf(line, "%63[^=]=%63s", key, value) == 2) {
            if (strcmp(key, "ROWS") == 0) cfg->ROWS = atoi(value);
            else if (strcmp(key, "COLS") == 0) cfg->COLS = atoi(value);
            else if (strcmp(key, "PLAYER_DELAY") == 0) cfg->PLAYER_DELAY = atoi(value);
            else if (strcmp(key, "CAR_DELAY") == 0) cfg->CAR_DELAY = atoi(value);
            else if (strcmp(key, "STORK_DELAY") == 0) cfg->STORK_DELAY = atoi(value);
            else if (strcmp(key, "MAX_CARS") == 0) cfg->MAX_CARS = atoi(value);
            else if (strcmp(key, "RIVERS") == 0) cfg->RIVERS = atoi(value);
            else if (strcmp(key, "SEED") == 0) cfg->SEED = atoi(value);
            else if (strcmp(key, "RENDER_MODE_2X2") == 0) cfg->RENDER_MODE_2X2 = atoi(value);
            else if (strcmp(key, "TREE_CHANCE") == 0) cfg->TREE_CHANCE = atoi(value);

        }
    }

    fclose(file);
}

void load_graphics1(const char *filename, Config * cfg) {
    FILE *file = fopen(filename, "r");

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char key[64];
        char value[64];
        if (sscanf(line, "%63[^=]=%63s", key, value) == 2) {
            if (strcmp(key, "SYMBOL_WATER") == 0) cfg->SYMBOL_WATER = value[0];
            else if (strcmp(key, "SYMBOL_FOREST") == 0) cfg->SYMBOL_FOREST = value[0];
            else if (strcmp(key, "SYMBOL_FINISH") == 0) cfg->SYMBOL_FINISH = value[0];
            else if (strcmp(key, "SYMBOL_TREE") == 0) cfg->SYMBOL_TREE = value[0];
            else if (strcmp(key, "SYMBOL_CAR") == 0) cfg->SYMBOL_CAR = value[0];
            else if (strcmp(key, "SYMBOL_FRIENDLY_CAR") == 0) cfg->SYMBOL_FRIENDLY_CAR = value[0];
            else if (strcmp(key, "SYMBOL_TAXI") == 0) cfg->SYMBOL_TAXI = value[0];
            else if (strcmp(key, "SYMBOL_PLAYER") == 0) cfg->SYMBOL_PLAYER = value[0];
            else if (strcmp(key, "SYMBOL_STORK") == 0) cfg->SYMBOL_STORK= value[0];
        }
    }

    fclose(file);
}

void assign_2x2_sprite(const char *value, char sprite[2][2]) {
    sscanf(value, "%c%c,%c%c",
           &sprite[0][0], &sprite[0][1],
           &sprite[1][0], &sprite[1][1]);
}

void load_graphics2(const char *filename, Config * cfg) {
    FILE *file = fopen(filename, "r");

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char k[64];
        char v[64];
        if (sscanf(line, "%63[^=]=%63s", k, v) == 2) {
            if (strcmp(k, "SYMBOL_WATER_2X2") == 0)
                assign_2x2_sprite(v, cfg->SYMBOL_WATER_2X2);
            else if (strcmp(k, "SYMBOL_FOREST_2X2") == 0)
                assign_2x2_sprite(v, cfg->SYMBOL_FOREST_2X2);
            else if (strcmp(k, "SYMBOL_FINISH_2X2") == 0)
                assign_2x2_sprite(v, cfg->SYMBOL_FINISH_2X2);
            else if (strcmp(k, "SYMBOL_TREE_2X2") == 0)
                assign_2x2_sprite(v, cfg->SYMBOL_TREE_2X2);
            else if (strcmp(k, "SYMBOL_CAR_2X2") == 0)
                assign_2x2_sprite(v, cfg->SYMBOL_CAR_2X2);
            else if (strcmp(k, "SYMBOL_FRIENDLY_CAR_2X2") == 0)
                assign_2x2_sprite(v, cfg->SYMBOL_FRIENDLY_CAR_2X2);
            else if (strcmp(k, "SYMBOL_TAXI_2X2") == 0)
                assign_2x2_sprite(v, cfg->SYMBOL_TAXI_2X2);
            else if (strcmp(k, "SYMBOL_PLAYER_2X2") == 0)
                assign_2x2_sprite(v, cfg->SYMBOL_PLAYER_2X2);
            else if (strcmp(k, "SYMBOL_STORK_2X2") == 0)
                assign_2x2_sprite(v, cfg->SYMBOL_STORK_2X2);
        }
    }

    fclose(file);
}

void load_config(Config * cfg){
    load_options("options.txt", cfg);
    load_graphics1("graphics1x1.txt", cfg);
    load_graphics2("graphics2x2.txt", cfg);

    //load colors
    cfg->COLOR_ROAD = 1;
    cfg->COLOR_WATER = 2;
    cfg->COLOR_FOREST = 3;
    cfg->COLOR_FINISH = 20;

    cfg->COLOR_TREE = 4;
    cfg->COLOR_CAR = 5;
    cfg->COLOR_FRIENDLY_CAR = 6;
    cfg->COLOR_PLAYER = 10;
    cfg->COLOR_STORK = 20;
}
