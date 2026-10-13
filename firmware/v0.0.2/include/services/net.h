#pragma once
#include <Arduino.h>

namespace net {
void begin();            // blocking connect (up to WIFI_TIMEOUT_MS)
void reconnect();        // non-blocking restart of the connection
bool connected();
String ssid();
String ip();
int rssi();
}
