#pragma once
#include <time.h>
#include "services/pet.h"

// 0.96" OLED pet display with time, weather, sensor, WiFi, and mute status.
namespace oledstatus {
void begin();
void update(const tm& t, bool haveTime);   // self-throttled to 1 Hz; call every loop
void update(const tm& t, bool haveTime, Pet& pet, const SensorState& sensors);
}
