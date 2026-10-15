#pragma once
#include <Arduino.h>

// pins 
// Buttons: 0 = UP, 1 = DOWN, 2 = SELECT (hold SELECT = back to menu)
constexpr int BTN[3] = {13, 14, 32};
constexpr int LED_R = 4, LED_G = 25, LED_B = 33;
constexpr int BUZZER = 15;
constexpr int SENSOR_TRIG = 16;
constexpr int SENSOR_ECHO = 17;  // Level-shift the HC-SR04 echo to 3.3 V.
constexpr int SENSOR_PIR = 34;   // Input-only GPIO.

// screen geometry (portrait) 
constexpr int ROTATION = 0;   // 0 = portrait
constexpr int SCREEN_H = 480;
constexpr int SCREEN_W = 320;
constexpr int BAR_H    = 60;
constexpr int HINT_Y   = 440;

// colours
constexpr uint16_t rgb(uint8_t r, uint8_t g, uint8_t b) {
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}
constexpr uint16_t COL_BAR  = rgb(24, 26, 34);
constexpr uint16_t COL_TILE = rgb(28, 30, 38);
constexpr uint16_t COL_EDGE = rgb(60, 64, 76);

// timing 
constexpr unsigned long WEATHER_REFRESH_MS = 15UL * 60 * 1000;
constexpr unsigned long WIFI_TIMEOUT_MS    = 15000;
