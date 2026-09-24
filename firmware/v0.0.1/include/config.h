#pragma once
#include <Arduino.h>

constexpr uint8_t PIN_TRIG   = 18;
constexpr uint8_t PIN_ECHO   = 19;   
constexpr uint8_t PIN_PIR    = 27;
constexpr uint8_t PIN_BTN1   = 13;   
constexpr uint8_t PIN_BTN2   = 14;   
constexpr uint8_t PIN_BUZZER = 23;
constexpr uint8_t PIN_LED_R  = 5;
constexpr uint8_t PIN_LED_G  = 32;
constexpr uint8_t PIN_LED_B  = 33;

constexpr uint8_t PIN_OLED_SDA = 21;
constexpr uint8_t PIN_OLED_SCL = 22;

constexpr uint8_t CH_R = 0;
constexpr uint8_t CH_G = 1;
constexpr uint8_t CH_B = 2;
constexpr uint32_t PWM_FREQ = 5000;
constexpr uint8_t  PWM_RES  = 8;

constexpr unsigned long ULTRASONIC_INTERVAL_MS = 60; 
constexpr unsigned long DEBOUNCE_MS = 30;

enum class RobotState { IDLE, ALERT, DANGER };

struct Mood {
  const char* name;
  float dangerCm;
};

extern Mood moods[];
extern const uint8_t moodCount;