#ifndef DIRECTION_H
#define DIRECTION_H

typedef enum {
    NORTH = 0,
    EAST =  1,
    SOUTH = 2,
    WEST =  3
} Direction;

Direction turn_left(Direction current);
Direction turn_right(Direction current);
const char* to_string(Direction dir);

#endif