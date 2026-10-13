#pragma once
#include <time.h>

namespace timesvc {
void begin();                              // start NTP (needs WiFi) and note boot time
// Fills t / haveTime. Returns true when the NTP-synced state just changed
// (so the UI should do a full redraw).
bool poll(tm& t, bool& haveTime);
unsigned long uptimeSec();                 // seconds since begin()
}
