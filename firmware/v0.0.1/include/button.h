#pragma once
#include <Arduino.h>

struct Button {
  uint8_t pin;
  bool lastReading;
  bool stableState;
  unsigned long lastChangeMs;
};

Button makeButton(uint8_t pin);
void buttonInit(const Button &b);
bool updateButton(Button &b);