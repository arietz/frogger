#ifndef ENTITIES_H
#define ENTITIES_H

typedef struct {
    int row;
    int col;
    int exists;
    int finished;
} EntityPlayer;

typedef struct {
    int row;
    int col;
    int direction;  //for direction = 0, car goes left, direction = 1, car goes right
    int speed;      //represented by how many CAR_DELAYS is needed to move, 
                    //e.g. speed = 2 means that car can move every 2 * CAR_DELAY milliseconds

    int counter;    //for counting how many CAR_DELAYS have passed
    int exists;
    int type;
} EntityCar;

#endif // ENTITIES_H