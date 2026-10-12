#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <TFT_eSPI.h>
#include <U8g2lib.h>
#include <time.h>
#include "id.h"

// ============================================================
//  Pins
// ============================================================
// Buttons: 0 = UP, 1 = DOWN, 2 = SELECT (hold SELECT = back to menu)
const int BTN[3] = {13, 14, 32};
const int LED_R = 4, LED_G = 25, LED_B = 33;
const int BUZZER = 15;

TFT_eSPI tft;
U8G2_SSD1306_128X64_NONAME_F_HW_I2C oled(U8G2_R0, U8X8_PIN_NONE);

// ============================================================
//  Screen geometry (portrait)
// ============================================================
constexpr int ROTATION = 0;   // 0 = portrait; use 2 if the image is upside down
constexpr int SCREEN_W = 320;
constexpr int SCREEN_H = 480;
constexpr int BAR_H    = 60;  // menu top bar
constexpr int HINT_Y   = 440; // two-line hint area at the bottom

// ============================================================
//  Colors
// ============================================================
constexpr uint16_t rgb(uint8_t r, uint8_t g, uint8_t b) {
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}
const uint16_t COL_BAR  = rgb(24, 26, 34);
const uint16_t COL_TILE = rgb(28, 30, 38);
const uint16_t COL_EDGE = rgb(60, 64, 76);

// ============================================================
//  Screens and menu tiles (2 columns x 3 rows)
// ============================================================
enum Screen { SCR_MENU, SCR_CLOCK, SCR_WEATHER, SCR_LIGHTS, SCR_SOUND, SCR_NETWORK, SCR_SYSTEM };

struct Tile { const char* label; uint16_t color; Screen target; };
const Tile TILES[] = {
  {"Clock",   TFT_CYAN,    SCR_CLOCK},
  {"Weather", TFT_ORANGE,  SCR_WEATHER},
  {"Lights",  TFT_YELLOW,  SCR_LIGHTS},
  {"Sound",   TFT_GREEN,   SCR_SOUND},
  {"Network", TFT_SKYBLUE, SCR_NETWORK},
  {"System",  TFT_MAGENTA, SCR_SYSTEM},
};
const int TILE_COUNT = sizeof(TILES) / sizeof(TILES[0]);

Screen screen = SCR_MENU;
int menuIndex = 0;
bool redraw = true;          // true = current screen needs a full redraw
unsigned long lastInfo = 0;  // refresh timer for the live info screens

// ============================================================
//  State
// ============================================================
bool muted = false;

// weather
float curTemp = NAN;
int curCode = -1;
float hi[3], lo[3];
bool haveWeather = false;
unsigned long lastWeather = 0;
uint32_t weatherRev = 0;  // bumps on every successful fetch

// lights
enum LightMode { LM_AUTO, LM_OFF, LM_RED, LM_GREEN, LM_BLUE, LM_YELLOW, LM_CYAN, LM_MAGENTA, LM_WHITE, LM_PARTY, LM_COUNT };
const char* LIGHT_NAMES[LM_COUNT] = {"Auto (weather)", "Off", "Red", "Green", "Blue",
                                     "Yellow", "Cyan", "Magenta", "White", "Party"};
int lightMode = LM_AUTO;
bool ledState[3] = {false, false, false};

// ============================================================
//  Basic helpers
// ============================================================
void beep(int ms = 40) {
  if (muted) return;
  digitalWrite(BUZZER, HIGH); delay(ms); digitalWrite(BUZZER, LOW);
}

void setLed(bool r, bool g, bool b) {
  ledState[0] = r; ledState[1] = g; ledState[2] = b;
  digitalWrite(LED_R, r); digitalWrite(LED_G, g); digitalWrite(LED_B, b);
}

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

String clip(String s, size_t n) {
  if (s.length() > n) s = s.substring(0, n - 1) + "~";
  return s;
}

void ledForWeather() {
  if (!haveWeather) { setLed(0, 0, 0); return; }
  if (curTemp <= 0)       setLed(0, 0, 1);  // freezing: blue
  else if (curTemp < 25)  setLed(0, 1, 0);  // mild: green
  else                    setLed(1, 0, 0);  // hot: red
}

void applyLightMode() {
  switch (lightMode) {
    case LM_AUTO:    ledForWeather();   break;
    case LM_OFF:     setLed(0, 0, 0);   break;
    case LM_RED:     setLed(1, 0, 0);   break;
    case LM_GREEN:   setLed(0, 1, 0);   break;
    case LM_BLUE:    setLed(0, 0, 1);   break;
    case LM_YELLOW:  setLed(1, 1, 0);   break;
    case LM_CYAN:    setLed(0, 1, 1);   break;
    case LM_MAGENTA: setLed(1, 0, 1);   break;
    case LM_WHITE:   setLed(1, 1, 1);   break;
    case LM_PARTY:   break;  // animated in loop()
  }
}

bool fetchWeather() {
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
  curTemp = doc["current"]["temperature_2m"];
  curCode = doc["current"]["weather_code"];
  for (int i = 0; i < 3; i++) {
    hi[i] = doc["daily"]["temperature_2m_max"][i];
    lo[i] = doc["daily"]["temperature_2m_min"][i];
  }
  haveWeather = true;
  weatherRev++;
  return true;
}

// ============================================================
//  Drawing helpers (320 x 480 portrait)
// ============================================================
void drawHeader(const char* title) {
  tft.fillRect(0, 0, SCREEN_W, 40, COL_BAR);
  tft.setTextColor(TFT_WHITE, COL_BAR);
  tft.setTextDatum(ML_DATUM);
  tft.drawString(title, 12, 20, 4);
}

// One or two centered lines in the hint area at the bottom
void drawHint(const char* l1, const char* l2 = nullptr) {
  tft.fillRect(0, HINT_Y, SCREEN_W, SCREEN_H - HINT_Y, TFT_BLACK);
  tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  if (l2) {
    tft.drawString(l1, SCREEN_W / 2, HINT_Y + 12, 2);
    tft.drawString(l2, SCREEN_W / 2, HINT_Y + 30, 2);
  } else {
    tft.drawString(l1, SCREEN_W / 2, HINT_Y + 20, 2);
  }
}

// Two-line info card: small gray label, larger white value underneath
void drawInfoRow(int y, const char* label, const String& value) {
  tft.fillRect(0, y, SCREEN_W, 56, TFT_BLACK);
  tft.setTextDatum(TL_DATUM);
  tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
  tft.drawString(label, 16, y, 2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString(value.c_str(), 16, y + 20, 4);
}

// ---------- menu dashboard ----------
void drawIcon(int i, int cx, int cy, uint16_t c) {
  switch (i) {
    case 0:  // clock
      tft.drawCircle(cx, cy, 20, c);
      tft.drawCircle(cx, cy, 19, c);
      tft.drawLine(cx, cy, cx, cy - 13, c);
      tft.drawLine(cx, cy, cx + 9, cy + 4, c);
      break;
    case 1:  // weather (sun)
      tft.fillCircle(cx, cy, 9, c);
      for (int a = 0; a < 8; a++) {
        float r = a * PI / 4;
        tft.drawLine(cx + (int)(cosf(r) * 14), cy + (int)(sinf(r) * 14),
                     cx + (int)(cosf(r) * 21), cy + (int)(sinf(r) * 21), c);
      }
      break;
    case 2:  // lights (bulb)
      tft.fillCircle(cx, cy - 5, 14, c);
      tft.fillRect(cx - 7, cy + 8, 14, 5, c);
      tft.fillRect(cx - 4, cy + 14, 8, 4, c);
      break;
    case 3:  // sound (speaker)
      tft.fillRect(cx - 18, cy - 6, 9, 12, c);
      tft.fillTriangle(cx - 9, cy - 6, cx + 3, cy - 16, cx + 3, cy + 16, c);
      tft.fillTriangle(cx - 9, cy - 6, cx - 9, cy + 6, cx + 3, cy + 16, c);
      tft.drawLine(cx + 9, cy - 8, cx + 9, cy + 8, c);
      tft.drawLine(cx + 15, cy - 13, cx + 15, cy + 13, c);
      break;
    case 4:  // network (signal bars)
      for (int k = 0; k < 4; k++)
        tft.fillRect(cx - 20 + k * 11, cy + 16 - (k + 1) * 9, 8, (k + 1) * 9, c);
      break;
    case 5:  // system (chip)
      tft.fillRect(cx - 12, cy - 12, 24, 24, c);
      for (int k = 0; k < 3; k++) {
        int o = -8 + k * 8;
        tft.drawFastHLine(cx - 20, cy + o, 8, c);
        tft.drawFastHLine(cx + 12, cy + o, 8, c);
        tft.drawFastVLine(cx + o, cy - 20, 8, c);
        tft.drawFastVLine(cx + o, cy + 12, 8, c);
      }
      break;
  }
}

// Tiles are 142 x 112, two per row; rows start at y = 68, 192, 316
void drawTile(int i, bool sel) {
  int x = 12 + (i % 2) * 154;
  int y = 68 + (i / 2) * 124;
  uint16_t base = TILES[i].color;
  uint16_t bg   = sel ? base : COL_TILE;
  uint16_t fg   = sel ? TFT_BLACK : base;
  uint16_t txt  = sel ? TFT_BLACK : TFT_WHITE;

  tft.fillRoundRect(x, y, 142, 112, 10, bg);
  if (!sel) tft.drawRoundRect(x, y, 142, 112, 10, COL_EDGE);
  drawIcon(i, x + 71, y + 44, fg);
  tft.setTextColor(txt, bg);
  tft.setTextDatum(MC_DATUM);
  tft.drawString(TILES[i].label, x + 71, y + 90, 4);
}

void drawTopBar(const tm& t, bool haveTime, bool wifi) {
  char buf[32];
  tft.fillRect(0, 0, SCREEN_W, BAR_H, COL_BAR);

  // row 1: time (left), WiFi indicator (right)
  if (haveTime) snprintf(buf, sizeof buf, "%02d:%02d", t.tm_hour, t.tm_min);
  else          snprintf(buf, sizeof buf, "--:--");
  tft.setTextColor(TFT_WHITE, COL_BAR);
  tft.setTextDatum(ML_DATUM);
  tft.drawString(buf, 12, 20, 4);

  tft.setTextColor(TFT_DARKGREY, COL_BAR);
  tft.setTextDatum(MR_DATUM);
  tft.drawString("WiFi", 286, 20, 2);
  tft.fillCircle(300, 20, 5, wifi ? TFT_GREEN : TFT_RED);

  // row 2: weather (left), date (right)
  if (haveWeather) snprintf(buf, sizeof buf, "%.0fC %s", curTemp, codeToText(curCode));
  else             snprintf(buf, sizeof buf, "no weather");
  tft.setTextColor(TFT_ORANGE, COL_BAR);
  tft.setTextDatum(ML_DATUM);
  tft.drawString(buf, 12, 45, 2);

  if (haveTime) {
    strftime(buf, sizeof buf, "%a %d %b", &t);
    tft.setTextColor(TFT_WHITE, COL_BAR);
    tft.setTextDatum(MR_DATUM);
    tft.drawString(buf, SCREEN_W - 12, 45, 2);
  }
}

void renderMenu(const tm& t, bool haveTime) {
  static int lastMinute = -2;
  static uint32_t lastRev = 0;
  static bool lastWifi = false;
  bool wifi = WiFi.status() == WL_CONNECTED;
  int minute = haveTime ? t.tm_min : -1;

  if (redraw) {
    tft.fillScreen(TFT_BLACK);
    for (int i = 0; i < TILE_COUNT; i++) drawTile(i, i == menuIndex);
    drawHint("UP/DOWN: move", "SELECT: open");
    lastMinute = -2;  // force the top bar to draw
  }
  if (lastMinute != minute || lastRev != weatherRev || lastWifi != wifi) {
    lastMinute = minute; lastRev = weatherRev; lastWifi = wifi;
    drawTopBar(t, haveTime, wifi);
  }
}

// ---------- clock ----------
void drawClockWaiting() {
  tft.fillScreen(TFT_BLACK);
  drawHeader("CLOCK");
  drawHint("HOLD SELECT: back");
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("Syncing time...", SCREEN_W / 2, 240, 4);
}

// Small weather summary under the clock
void drawClockWeather() {
  tft.fillRect(0, 280, SCREEN_W, 90, TFT_BLACK);
  if (!haveWeather) return;
  char buf[24];
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(TFT_ORANGE, TFT_BLACK);
  snprintf(buf, sizeof buf, "%.1f C", curTemp);
  tft.drawString(buf, SCREEN_W / 2, 305, 4);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString(codeToText(curCode), SCREEN_W / 2, 338, 4);
}

void drawClock(const tm& t, bool full) {
  static int lastMin = -1, lastSec = -1;
  static uint32_t lastRev = 0xFFFFFFFF;
  char buf[16];

  if (full) {
    tft.fillScreen(TFT_BLACK);
    drawHeader("CLOCK");
    drawHint("HOLD SELECT: back");
    lastMin = lastSec = -1;
    lastRev = 0xFFFFFFFF;
  }
  tft.setTextDatum(MC_DATUM);

  if (t.tm_min != lastMin) {
    lastMin = t.tm_min;
    tft.fillRect(0, 85, SCREEN_W, 70, TFT_BLACK);
    tft.setTextColor(TFT_CYAN, TFT_BLACK);
    snprintf(buf, sizeof buf, "%02d:%02d", t.tm_hour, t.tm_min);
    tft.drawString(buf, SCREEN_W / 2, 120, 7);

    strftime(buf, sizeof buf, "%a %d %b", &t);
    tft.fillRect(0, 175, SCREEN_W, 36, TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.drawString(buf, SCREEN_W / 2, 192, 4);
  }
  if (t.tm_sec != lastSec) {
    lastSec = t.tm_sec;
    tft.fillRect(110, 225, 100, 30, TFT_BLACK);
    tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
    snprintf(buf, sizeof buf, "%02d s", t.tm_sec);
    tft.drawString(buf, SCREEN_W / 2, 240, 4);
  }
  if (lastRev != weatherRev) {
    lastRev = weatherRev;
    drawClockWeather();
  }
}

// ---------- weather ----------
void drawWeather() {
  tft.fillScreen(TFT_BLACK);
  drawHeader("WEATHER");
  drawHint("SELECT: refresh", "HOLD SELECT: back");

  if (!haveWeather) {
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    tft.setTextDatum(MC_DATUM);
    tft.drawString("No weather data yet", SCREEN_W / 2, 240, 4);
    return;
  }

  char buf[32];

  // big temperature number (font 7) + unit (font 4), centered together
  snprintf(buf, sizeof buf, "%.1f", curTemp);
  int w  = tft.textWidth(buf, 7);
  int cw = tft.textWidth("C", 4);
  int x0 = (SCREEN_W - (w + 10 + cw)) / 2;
  tft.setTextColor(TFT_ORANGE, TFT_BLACK);
  tft.setTextDatum(ML_DATUM);
  tft.drawString(buf, x0, 100, 7);
  tft.drawString("C", x0 + w + 10, 100, 4);

  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  tft.drawString(codeToText(curCode), SCREEN_W / 2, 165, 4);

  // 3-day list
  tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
  tft.setTextDatum(MR_DATUM);
  tft.drawString("HIGH / LOW", 300, 210, 2);

  const char* labels[3] = {"Today", "Tomorrow", "Day 3"};
  for (int i = 0; i < 3; i++) {
    int yc = 245 + i * 52;
    tft.drawFastHLine(16, yc - 26, SCREEN_W - 32, COL_EDGE);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextDatum(ML_DATUM);
    tft.drawString(labels[i], 20, yc, 4);
    snprintf(buf, sizeof buf, "%.0f / %.0f", hi[i], lo[i]);
    tft.setTextDatum(MR_DATUM);
    tft.drawString(buf, 300, yc, 4);
  }
}

// ---------- lights ----------
const int SWATCH_X = SCREEN_W / 2, SWATCH_Y = 170, SWATCH_R = 70;

void drawLightsSwatch() {
  uint16_t c = rgb(ledState[0] * 255, ledState[1] * 255, ledState[2] * 255);
  tft.fillCircle(SWATCH_X, SWATCH_Y, SWATCH_R + 6, TFT_BLACK);
  if (ledState[0] || ledState[1] || ledState[2]) tft.fillCircle(SWATCH_X, SWATCH_Y, SWATCH_R, c);
  tft.drawCircle(SWATCH_X, SWATCH_Y, SWATCH_R + 3, TFT_DARKGREY);
}

void drawLights() {
  tft.fillScreen(TFT_BLACK);
  drawHeader("LIGHTS");
  drawLightsSwatch();
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  tft.drawString(LIGHT_NAMES[lightMode], SCREEN_W / 2, 290, 4);
  drawHint("UP/DOWN: change mode", "HOLD SELECT: back");
}

// ---------- sound ----------
void drawSound() {
  tft.fillScreen(TFT_BLACK);
  drawHeader("SOUND");
  tft.setTextColor(muted ? TFT_RED : TFT_GREEN, TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  tft.setTextSize(2);
  tft.drawString(muted ? "MUTED" : "ON", SCREEN_W / 2, 200, 4);
  tft.setTextSize(1);
  tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
  tft.drawString("Beeps on button presses", SCREEN_W / 2, 265, 2);
  drawHint("SELECT: mute   UP/DOWN: test beep", "HOLD SELECT: back");
}

// ---------- network ----------
void drawNetwork(bool full) {
  if (full) {
    tft.fillScreen(TFT_BLACK);
    drawHeader("NETWORK");
    drawHint("SELECT: reconnect", "HOLD SELECT: back");
  }
  bool ok = WiFi.status() == WL_CONNECTED;
  char buf[24];
  drawInfoRow(56,  "Status",     ok ? "Connected" : "Offline");
  drawInfoRow(120, "SSID",       ok ? clip(WiFi.SSID(), 20) : String("-"));
  drawInfoRow(184, "IP address", ok ? WiFi.localIP().toString() : String("-"));
  if (ok) snprintf(buf, sizeof buf, "%d dBm", WiFi.RSSI());
  else    snprintf(buf, sizeof buf, "-");
  drawInfoRow(248, "Signal",     buf);
  drawInfoRow(312, "Time sync",  time(nullptr) > 1600000000L ? "Synced" : "Waiting");
}

// ---------- system ----------
void drawSystem(bool full) {
  if (full) {
    tft.fillScreen(TFT_BLACK);
    drawHeader("SYSTEM");
    drawHint("HOLD SELECT: back");
  }
  char buf[32];
  snprintf(buf, sizeof buf, "%s r%d", ESP.getChipModel(), (int)ESP.getChipRevision());
  drawInfoRow(56, "Chip", buf);
  snprintf(buf, sizeof buf, "%u MHz", (unsigned)getCpuFrequencyMhz());
  drawInfoRow(120, "CPU speed", buf);
  snprintf(buf, sizeof buf, "%u KB", (unsigned)(ESP.getFreeHeap() / 1024));
  drawInfoRow(184, "Free heap", buf);
  snprintf(buf, sizeof buf, "%u MB", (unsigned)(ESP.getFlashChipSize() / (1024 * 1024)));
  drawInfoRow(248, "Flash", buf);
  unsigned long s = millis() / 1000;
  snprintf(buf, sizeof buf, "%02lu:%02lu:%02lu", s / 3600, (s / 60) % 60, s % 60);
  drawInfoRow(312, "Uptime", buf);
}

// ============================================================
//  OLED status (unchanged)
// ============================================================
void updateOled(const tm& t, bool haveTime) {
  char buf[24];
  oled.clearBuffer();
  oled.setFont(u8g2_font_ncenB14_tr);
  if (haveTime) snprintf(buf, sizeof buf, "%02d:%02d:%02d", t.tm_hour, t.tm_min, t.tm_sec);
  else          snprintf(buf, sizeof buf, "--:--:--");
  oled.drawStr(8, 22, buf);
  oled.setFont(u8g2_font_6x12_tr);
  if (haveWeather) {
    snprintf(buf, sizeof buf, "%.1fC %s", curTemp, codeToText(curCode));
    oled.drawStr(0, 42, buf);
  } else oled.drawStr(0, 42, "weather: --");
  snprintf(buf, sizeof buf, "WiFi:%s  Mute:%s",
           WiFi.status() == WL_CONNECTED ? "ok" : "no", muted ? "on" : "off");
  oled.drawStr(0, 60, buf);
  oled.sendBuffer();
}

// ============================================================
//  Buttons
// ============================================================
enum Btn { B_NONE, B_UP, B_DOWN, B_SELECT, B_BACK };

// UP / DOWN fire on press. SELECT fires on release (short press) or
// as BACK once it has been held for 600 ms.
Btn readButtons() {
  static bool prev[3] = {false, false, false};
  static unsigned long changed[3] = {0, 0, 0};
  static unsigned long pressedAt = 0;
  static bool longFired = false;
  unsigned long now = millis();

  for (int i = 0; i < 3; i++) {
    bool down = digitalRead(BTN[i]) == LOW;
    if (down != prev[i] && now - changed[i] > 30) {  // 30 ms debounce
      changed[i] = now;
      prev[i] = down;
      if (i == 0 && down) return B_UP;
      if (i == 1 && down) return B_DOWN;
      if (i == 2) {
        if (down) { pressedAt = now; longFired = false; }
        else if (!longFired) return B_SELECT;
      }
    }
  }
  if (prev[2] && !longFired && now - pressedAt > 600) {
    longFired = true;
    return B_BACK;
  }
  return B_NONE;
}

void goTo(Screen s) {
  screen = s;
  redraw = true;
  lastInfo = 0;
  beep(30);
}

// Button actions inside a (non-menu) screen
void screenButton(Btn b) {
  switch (screen) {
    case SCR_WEATHER:
      if (b == B_SELECT) {
        if (WiFi.status() == WL_CONNECTED) {
          drawHint("Refreshing...");
          if (fetchWeather()) {
            lastWeather = millis();
            if (lightMode == LM_AUTO) ledForWeather();
          }
          redraw = true;
        } else {
          drawHint("WiFi not connected");
        }
      }
      break;

    case SCR_LIGHTS:
      if (b == B_UP || b == B_DOWN) {
        lightMode = (lightMode + (b == B_DOWN ? 1 : LM_COUNT - 1)) % LM_COUNT;
        applyLightMode();
        redraw = true;
        beep(15);
      }
      break;

    case SCR_SOUND:
      if (b == B_SELECT) {
        muted = !muted;
        redraw = true;
        beep(100);  // audible only when just unmuted
      } else if (b == B_UP || b == B_DOWN) {
        beep(60);
      }
      break;

    case SCR_NETWORK:
      if (b == B_SELECT) {
        WiFi.disconnect();
        WiFi.begin(WIFI_SSID, WIFI_PASS);
        lastInfo = 0;
        beep(30);
      }
      break;

    default:
      break;
  }
}

void handleButton(Btn b) {
  if (screen == SCR_MENU) {
    if (b == B_UP || b == B_DOWN) {
      int old = menuIndex;
      menuIndex = (menuIndex + (b == B_DOWN ? 1 : TILE_COUNT - 1)) % TILE_COUNT;
      drawTile(old, false);
      drawTile(menuIndex, true);
      beep(15);
    } else if (b == B_SELECT) {
      goTo(TILES[menuIndex].target);
    }
    return;
  }
  if (b == B_BACK) { goTo(SCR_MENU); return; }
  screenButton(b);
}

// ============================================================
//  Rendering dispatcher
// ============================================================
void render(const tm& t, bool haveTime) {
  static bool clockNeedsFull = true;

  switch (screen) {
    case SCR_MENU:
      renderMenu(t, haveTime);
      break;

    case SCR_CLOCK:
      if (haveTime) {
        drawClock(t, redraw || clockNeedsFull);
        clockNeedsFull = false;
      } else if (redraw) {
        drawClockWaiting();
        clockNeedsFull = true;
      }
      break;

    case SCR_WEATHER:
      if (redraw) drawWeather();
      break;

    case SCR_LIGHTS:
      if (redraw) drawLights();
      break;

    case SCR_SOUND:
      if (redraw) drawSound();
      break;

    case SCR_NETWORK:
      if (redraw || millis() - lastInfo > 2000) { drawNetwork(redraw); lastInfo = millis(); }
      break;

    case SCR_SYSTEM:
      if (redraw || millis() - lastInfo > 1000) { drawSystem(redraw); lastInfo = millis(); }
      break;
  }
  redraw = false;
}

// ============================================================
//  Arduino entry points
// ============================================================
void setup() {
  Serial.begin(115200);
  for (int p : BTN) pinMode(p, INPUT_PULLUP);
  pinMode(LED_R, OUTPUT); pinMode(LED_G, OUTPUT); pinMode(LED_B, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  tft.init(); tft.setRotation(ROTATION); tft.fillScreen(TFT_BLACK);
  oled.begin();

  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString("Connecting WiFi...", 20, 20, 4);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  unsigned long t0 = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - t0 < 15000) delay(200);
  Serial.printf("WiFi: %s\n", WiFi.status() == WL_CONNECTED ? "connected" : "FAILED");

  configTzTime(TZ_INFO, "pool.ntp.org", "time.nist.gov");
  if (WiFi.status() == WL_CONNECTED && fetchWeather()) lastWeather = millis();
  applyLightMode();
  beep(80);
}

void loop() {
  static unsigned long lastOled = 0, lastParty = 0;
  static int partyStep = 0;

  // input
  Btn b = readButtons();
  if (b != B_NONE) handleButton(b);

  // weather refresh every 15 minutes
  if (WiFi.status() == WL_CONNECTED && millis() - lastWeather > 15UL * 60 * 1000) {
    lastWeather = millis();
    if (fetchWeather()) {
      if (lightMode == LM_AUTO) ledForWeather();
      if (screen == SCR_WEATHER) redraw = true;
    }
  }

  // party light animation
  if (lightMode == LM_PARTY && millis() - lastParty > 350) {
    lastParty = millis();
    static const bool cols[6][3] = {{1,0,0},{1,1,0},{0,1,0},{0,1,1},{0,0,1},{1,0,1}};
    partyStep = (partyStep + 1) % 6;
    setLed(cols[partyStep][0], cols[partyStep][1], cols[partyStep][2]);
    if (screen == SCR_LIGHTS) drawLightsSwatch();
  }

  // time (menu and buttons keep working even before NTP syncs)
  tm t = {};
  bool haveTime = getLocalTime(&t, 10);

  render(t, haveTime);

  if (millis() - lastOled > 1000) { lastOled = millis(); updateOled(t, haveTime); }
}