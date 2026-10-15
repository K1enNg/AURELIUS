#include <Arduino.h>
#include "services/timesvc.h"
#include "services/net.h"
#include "id.h"   // TZ_INFO

namespace timesvc {

static bool s_synced = false;
static unsigned long s_bootMillis = 0;

void begin() {
  s_bootMillis = millis();
  if (net::connected()) {
    configTzTime(TZ_INFO, "pool.ntp.org", "time.nist.gov");
    Serial.println("NTP synchronization started");
  } else {
    Serial.println("NTP unavailable: WiFi disconnected");
  }
}

bool poll(tm& t, bool& haveTime) {
  haveTime = getLocalTime(&t, 10);
  bool syncedNow = time(nullptr) > 1700000000L;
  bool changed = false;

  if (syncedNow && !s_synced) {
    s_synced = true;
    changed = true;
    Serial.println("NTP synchronized successfully");
  } else if (!syncedNow && s_synced) {
    s_synced = false;
    changed = true;
  }
  return changed;
}

unsigned long uptimeSec() { return (millis() - s_bootMillis) / 1000; }

}
