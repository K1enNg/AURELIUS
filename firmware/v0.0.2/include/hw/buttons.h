#pragma once

enum Btn { B_NONE, B_UP, B_DOWN, B_SELECT, B_BACK };

namespace buttons {
void begin();
Btn read();
}
