#include <stdio.h>
#include "robot.h"

Robot robot_init(void) {
    Robot bot = { .x = 0, .y = 0, .dir = NORTH};
    return bot;
}

void robot_turn_left(Robot *bot) {
    bot->dir = turn_left(bot->dir);
}

void robot_turn_right(Robot *bot) {
    bot->dir = turn_right(bot->dir);
}

void robot_move_forward(Robot *bot, int distance) {
    switch(bot->dir) {
        case NORTH: bot->y += distance; break;
        case EAST: bot->x += distance; break;
        case SOUTH: bot->y -= distance; break;
        case WEST: bot->x -= distance; break;
    }
}

void robot_print_status(const Robot *bot) {
    printf("Position: (%d, %d)\n", bot->x, bot->y);
    printf("Direction: (%s)\n", to_string(bot->dir));
}