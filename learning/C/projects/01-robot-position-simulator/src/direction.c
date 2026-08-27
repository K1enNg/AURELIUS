#include "direction.h"

Direction turn_left(Direction current) {
    return (Direction)((current + 3) % 4);
}

Direction turn_right(Direction current) {
    return (Direction)((current + 1) % 4);
}

const char* to_string(Direction dir) {
    static const char *names[] = {"North", "East", "South", "West"};
    if (dir >= NORTH && dir <= WEST) {
        return names[dir];
    }
    return "unknown";
}
