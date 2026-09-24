#include "button.h"
#include "config.h"

Button makeButton(uint8_t pin) {
  return Button{pin, HIGH, HIGH, 0};
}

void buttonInit(const Button &b) {
  pinMode(b.pin, INPUT_PULLUP);
}

bool updateButton(Button &b) {
  bool reading = digitalRead(b.pin);
  unsigned long now = millis();

  if (reading != b.lastReading) {
    b.lastChangeMs = now;
  }

  bool pressedEdge = false;
  if (now - b.lastChangeMs > DEBOUNCE_MS) {
    if (reading != b.stableState) {
      b.stableState = reading;
      if (b.stableState == LOW) { // active-low
        pressedEdge = true;
      }
    }
  }
  b.lastReading = reading;
  return pressedEdge;
}