#pragma once
#include <Arduino.h>

namespace weather {

struct Data {
  float temp = NAN;
  int code = -1;
  float hi[3] = {0, 0, 0};
  float lo[3] = {0, 0, 0};
  bool valid = false;
  uint32_t rev = 0;      // bumps on every successful fetch
};

const Data& data();
const char* codeToText(int code);

void begin();          // first fetch (if WiFi is up)
bool poll();           // call from loop(), true when new data arrived
bool refreshNow();     // manual refresh, true on success

} 
