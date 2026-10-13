#include "ui/screens/screens.h"
#include "ui/widgets.h"
#include "hw/display.h"
#include "hw/sound.h"
#include "services/net.h"
#include "config.h"

namespace screens {

static unsigned long s_lastUpdate = 0;

static void draw(bool full) {
  if (full) {
    tft.fillScreen(TFT_BLACK);
    widgets::drawHeader("NETWORK");
    widgets::drawHint("SELECT: reconnect", "HOLD SELECT: back");
  }
  bool ok = net::connected();
  char buf[24];
  widgets::drawInfoRow(56,  "Status",     ok ? "Connected" : "Offline");
  widgets::drawInfoRow(120, "SSID",       ok ? widgets::clip(net::ssid(), 20) : String("-"));
  widgets::drawInfoRow(184, "IP address", ok ? net::ip() : String("-"));
  if (ok) snprintf(buf, sizeof buf, "%d dBm", net::rssi());
  else    snprintf(buf, sizeof buf, "-");
  widgets::drawInfoRow(248, "Signal",     buf);
  widgets::drawInfoRow(312, "Time sync",  time(nullptr) > 1600000000L ? "Synced" : "Waiting");
}

static void render(const tm&, bool, bool full) {
  if (full || millis() - s_lastUpdate > 2000) {
    draw(full);
    s_lastUpdate = millis();
  }
}

static void onButton(Btn b) {
  if (b != B_SELECT) return;
  net::reconnect();
  s_lastUpdate = 0;   // refresh the info rows right away
  sound::beep(30);
}

const Def networkScreen = {render, onButton};

}  // namespace screens
