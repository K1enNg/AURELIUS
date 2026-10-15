#include <Arduino.h>
#include "services/lights.h"
#include "services/weather.h"
#include "config.h"

namespace lights {

static const char* NAMES[LM_COUNT] = {"Auto (weather)", "Off", "Red", "Green", "Blue",
                                      "Yellow", "Cyan", "Magenta", "White", "Party"};
static Mode s_mode = LM_AUTO;
static bool s_state[3] = {false, false, false};

static void setLed(bool r, bool g, bool b) {
  s_state[0] = r; s_state[1] = g; s_state[2] = b;
  digitalWrite(LED_R, r); digitalWrite(LED_G, g); digitalWrite(LED_B, b);
}

static void ledForWeather() {
  const weather::Data& w = weather::data();
  if (!w.valid)          setLed(0, 0, 0);
  else if (w.temp <= 0)  setLed(0, 0, 1);  // freezing: blue
  else if (w.temp < 25)  setLed(0, 1, 0);  // mild: green
  else                   setLed(1, 0, 0);  // hot: red
}

static void applyMode() {
  switch (s_mode) {
    case LM_AUTO:    ledForWeather();  break;
    case LM_OFF:     setLed(0, 0, 0);  break;
    case LM_RED:     setLed(1, 0, 0);  break;
    case LM_GREEN:   setLed(0, 1, 0);  break;
    case LM_BLUE:    setLed(0, 0, 1);  break;
    case LM_YELLOW:  setLed(1, 1, 0);  break;
    case LM_CYAN:    setLed(0, 1, 1);  break;
    case LM_MAGENTA: setLed(1, 0, 1);  break;
    case LM_WHITE:   setLed(1, 1, 1);  break;
    case LM_PARTY:   break;  // animated in update()
    default:         break;
  }
}

void begin() {
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);
}

void startupTest() {
  setLed(true, false, false);  delay(250);
  setLed(false, true, false);  delay(250);
  setLed(false, false, true);  delay(250);
  setLed(false, false, false);
  applyMode();
}

void cycle(int dir) {
  s_mode = (Mode)((s_mode + (dir > 0 ? 1 : LM_COUNT - 1)) % LM_COUNT);
  applyMode();
}

void update() {
  static unsigned long lastParty = 0;
  static int step = 0;
  if (s_mode != LM_PARTY || millis() - lastParty <= 350) return;
  lastParty = millis();
  static const bool cols[6][3] = {{1,0,0},{1,1,0},{0,1,0},{0,1,1},{0,0,1},{1,0,1}};
  step = (step + 1) % 6;
  setLed(cols[step][0], cols[step][1], cols[step][2]);
}

void onWeatherChanged() {
  if (s_mode == LM_AUTO) ledForWeather();
}

Mode mode()              { return s_mode; }
const char* modeName()   { return NAMES[s_mode]; }
bool channelOn(int ch)   { return s_state[ch]; }

}
