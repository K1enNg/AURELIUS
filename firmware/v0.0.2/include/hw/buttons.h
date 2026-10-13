#pragma once

enum Btn { B_NONE, B_UP, B_DOWN, B_SELECT, B_BACK };

namespace buttons {
void begin();
// UP / DOWN fire on press. SELECT fires on release (short press) or as
// B_BACK once it has been held for 600 ms.
Btn read();
}
