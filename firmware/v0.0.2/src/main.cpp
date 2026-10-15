#include <Arduino.h>
#include "config.h"
#include "hw/display.h"
#include "hw/buttons.h"
#include "hw/sound.h"
#include "services/net.h"
#include "services/timesvc.h"
#include "services/weather.h"
#include "services/lights.h"
#include "ui/ui.h"
#include "ui/oled_status.h"
#include "services/sensors.h"
#include "services/pet.h"

static Sensors sensors(SENSOR_TRIG, SENSOR_ECHO, SENSOR_PIR);
static Pet pet;

void setup() {
  Serial.begin(115200);
  buttons::begin();
  lights::begin();
  sound::begin();
  sensors.begin();
  pet.begin();

  displayBegin();
  oledstatus::begin();

  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString("Connecting WiFi...", 20, 20, 4);
  net::begin();
  timesvc::begin();
  weather::begin();

  lights::startupTest();   // flashes R/G/B, then restores the current light mode
  sound::beep(80);
}

void loop() {
  // input
  Btn b = buttons::read();
  if (b != B_NONE) ui::handleButton(b);

  // background services
  sensors.update();
  const SensorState& sensorState = sensors.state();
  pet.update(sensorState, millis());

  if (weather::poll()) {
    lights::onWeatherChanged();
    if (ui::current() == ui::SCR_WEATHER) ui::requestRedraw();
  }
  lights::update();

  // time (menu and buttons keep working even before NTP syncs)
  tm t = {};
  bool haveTime = false;
  if (timesvc::poll(t, haveTime)) ui::requestRedraw();

  // output
  ui::render(t, haveTime);
  oledstatus::update(t, haveTime, pet, sensorState);
}
