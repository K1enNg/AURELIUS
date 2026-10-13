#include <Arduino.h>
#include "hw/buttons.h"
#include "config.h"

namespace buttons {

void begin() {
  for (int p : BTN) pinMode(p, INPUT_PULLUP);
}

Btn read() {
  static bool prev[3] = {false, false, false};
  static unsigned long changed[3] = {0, 0, 0};
  static unsigned long pressedAt = 0;
  static bool longFired = false;
  unsigned long now = millis();

  for (int i = 0; i < 3; i++) {
    bool down = digitalRead(BTN[i]) == LOW;
    if (down != prev[i] && now - changed[i] > 30) {  // 30 ms debounce
      changed[i] = now;
      prev[i] = down;
      if (i == 0 && down) return B_UP;
      if (i == 1 && down) return B_DOWN;
      if (i == 2) {
        if (down) { pressedAt = now; longFired = false; }
        else if (!longFired) return B_SELECT;
      }
    }
  }
  if (prev[2] && !longFired && now - pressedAt > 600) {
    longFired = true;
    return B_BACK;
  }
  return B_NONE;
}

}
