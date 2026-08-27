#ifndef ROBOT_H
#define ROBOT_H

#include "direction.h"

typedef struct {
    int x;
    int y;
    Direction dir;
} Robot;

Robot robot_init(void);

void robot_turn_left(Robot *bot);
void robot_turn_right(Robot *bot);
void robot_move_forward(Robot *bot, int distance);

void robot_print_status(const Robot *bot);

#endif