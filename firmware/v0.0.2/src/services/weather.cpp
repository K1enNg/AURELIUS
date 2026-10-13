#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "services/weather.h"
#include "services/net.h"
#include "config.h"
#include "id.h"   // LATITUDE, LONGITUDE

namespace weather {

static Data s_data;
static unsigned long s_lastFetch = 0;

const Data& data() { return s_data; }

const char* codeToText(int c) {
  if (c == 0) return "Clear";
  if (c <= 3) return "Cloudy";
  if (c <= 48) return "Fog";
  if (c <= 57) return "Drizzle";
  if (c <= 67) return "Rain";
  if (c <= 77) return "Snow";
  if (c <= 82) return "Showers";
  if (c <= 86) return "Snow showers";
  return "Storm";
}

static bool fetch() {
  WiFiClientSecure client;
  client.setInsecure();
  HTTPClient http;
  String url = String("https://api.open-meteo.com/v1/forecast?latitude=") + LATITUDE +
               "&longitude=" + LONGITUDE +
               "&current=temperature_2m,weather_code"
               "&daily=temperature_2m_max,temperature_2m_min"
               "&timezone=auto&forecast_days=3";
  if (!http.begin(client, url)) return false;
  int code = http.GET();
  if (code != 200) { http.end(); return false; }
  JsonDocument doc;
  DeserializationError err = deserializeJson(doc, http.getString());
  http.end();
  if (err) return false;

  s_data.temp = doc["current"]["temperature_2m"];
  s_data.code = doc["current"]["weather_code"];
  for (int i = 0; i < 3; i++) {
    s_data.hi[i] = doc["daily"]["temperature_2m_max"][i];
    s_data.lo[i] = doc["daily"]["temperature_2m_min"][i];
  }
  s_data.valid = true;
  s_data.rev++;
  return true;
}

void begin() {
  if (net::connected() && fetch()) s_lastFetch = millis();
}

bool poll() {
  if (!net::connected() || millis() - s_lastFetch <= WEATHER_REFRESH_MS) return false;
  s_lastFetch = millis();   // also throttles retries after a failure
  return fetch();
}

bool refreshNow() {
  if (!fetch()) return false;
  s_lastFetch = millis();
  return true;
}

}  // namespace weather
