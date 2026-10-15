#pragma once
#include <Arduino.h>
#include <time.h>

namespace screens {

void alarmRingDraw(const tm& t, bool haveTime, bool full);

}