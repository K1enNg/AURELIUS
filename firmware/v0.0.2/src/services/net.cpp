#include <WiFi.h>
#include "services/net.h"
#include "config.h"
#include "id.h"   // WIFI_SSID, WIFI_PASS

namespace net {

void begin() {
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  unsigned long t0 = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - t0 < WIFI_TIMEOUT_MS) delay(200);
  Serial.printf("WiFi: %s\n", connected() ? "connected" : "FAILED");
}

void reconnect() {
  WiFi.disconnect();
  WiFi.begin(WIFI_SSID, WIFI_PASS);
}

bool connected() { return WiFi.status() == WL_CONNECTED; }
String ssid()    { return WiFi.SSID(); }
String ip()      { return WiFi.localIP().toString(); }
int rssi()       { return WiFi.RSSI(); }

}  // namespace net
