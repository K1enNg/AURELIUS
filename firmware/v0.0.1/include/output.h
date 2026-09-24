#pragma once
#include <Arduino.h>
#include "config.h"

void outputsInit();
void setColor(uint8_t r, uint8_t g, uint8_t b);
void updateLedForState(RobotState state, unsigned long now, unsigned long stateStartMs);
void updateBuzzer(unsigned long now, unsigned long intervalMs);
void startChirp(unsigned long now);
bool tickChirp(unsigned long now);
void buzzerOff();