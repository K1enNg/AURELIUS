#include <Arduino.h>
#include <U8g2lib.h>
#include "ui/oled_status.h"
#include "services/net.h"
#include "services/weather.h"
#include "hw/sound.h"

namespace oledstatus {

static U8G2_SSD1306_128X64_NONAME_F_HW_I2C oled(U8G2_R0, U8X8_PIN_NONE);

void begin() { oled.begin(); }

void update(const tm& t, bool haveTime) {
  static unsigned long last = 0;
  if (millis() - last <= 1000) return;
  last = millis();

  const weather::Data& w = weather::data();
  char buf[24];
  oled.clearBuffer();
  oled.setFont(u8g2_font_ncenB14_tr);
  if (haveTime) snprintf(buf, sizeof buf, "%02d:%02d:%02d", t.tm_hour, t.tm_min, t.tm_sec);
  else          snprintf(buf, sizeof buf, "--:--:--");
  oled.drawStr(8, 22, buf);
  oled.setFont(u8g2_font_6x12_tr);
  if (w.valid) {
    snprintf(buf, sizeof buf, "%.1fC %s", w.temp, weather::codeToText(w.code));
    oled.drawStr(0, 42, buf);
  } else oled.drawStr(0, 42, "weather: --");
  snprintf(buf, sizeof buf, "WiFi:%s  Mute:%s",
           net::connected() ? "ok" : "no", sound::isMuted() ? "on" : "off");
  oled.drawStr(0, 60, buf);
  oled.sendBuffer();
}

}
