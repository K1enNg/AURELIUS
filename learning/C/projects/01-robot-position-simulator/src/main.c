#include "robot.h"

int main(void) {
    Robot bot = robot_init();

    robot_move_forward(&bot, 10);
    robot_turn_left(&bot);
    robot_move_forward(&bot, 10);
    robot_turn_right(&bot);
    robot_move_forward(&bot, 10);

    roboprint_status(&bot);

    return 0;
}