#pragma once
#include <Arduino.h>
#include <time.h>

// Daily wake-up alarm: stored in flash, rings with a beep pattern, supports snooze.
namespace alarmclock {

constexpr uint8_t SNOOZE_MIN = 5;   // minutes added by a snooze

void begin();                                  // loads the saved alarm from flash
void update(const tm& t, bool haveTime);       // call every loop, after the time service

bool    enabled();
uint8_t hour();
uint8_t minute();
void adjustHour(int dir);                      // +1 / -1, wraps 23 <-> 0, enables the alarm
void adjustMinute(int dir, int step);          // +/- step minutes, wraps, enables the alarm
void setEnabled(bool on);
void toggle();

bool     ringing();
bool     snoozing();
uint32_t snoozeLeftS();
void snooze();                                 // silence now, ring again in SNOOZE_MIN minutes
void dismiss();                                // stop for today

}