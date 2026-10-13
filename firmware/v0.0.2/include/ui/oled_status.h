#pragma once
#include <time.h>

// 0.96" OLED status display (time, weather, WiFi / mute state).
namespace oledstatus {
void begin();
void update(const tm& t, bool haveTime);   // self-throttled to 1 Hz; call every loop
}
