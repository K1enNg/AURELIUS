#include "output.h"
#include <math.h>

// internal buzzer state, kept private to this module
static unsigned long s_lastBuzzMs = 0;
static bool s_buzzerOn = false;
static unsigned long s_chirpStartMs = 0;
static bool s_chirping = false;

void outputsInit() {
  pinMode(PIN_BUZZER, OUTPUT);
  digitalWrite(PIN_BUZZER, LOW);

  ledcSetup(CH_R, PWM_FREQ, PWM_RES);
  ledcSetup(CH_G, PWM_FREQ, PWM_RES);
  ledcSetup(CH_B, PWM_FREQ, PWM_RES);
  ledcAttachPin(PIN_LED_R, CH_R);
  ledcAttachPin(PIN_LED_G, CH_G);
  ledcAttachPin(PIN_LED_B, CH_B);
}

void setColor(uint8_t r, uint8_t g, uint8_t b) {
  ledcWrite(CH_R, r);
  ledcWrite(CH_G, g);
  ledcWrite(CH_B, b);
}

void updateLedForState(RobotState state, unsigned long now, unsigned long stateStartMs) {
  switch (state) {
    case RobotState::IDLE: {
      // slow blue "breathing" using a sine wave over time
      float t = (now - stateStartMs) / 1000.0f;
      uint8_t brightness = (uint8_t)((sin(t * 1.5f) * 0.5f + 0.5f) * 180);
      setColor(0, 0, brightness);
      break;
    }
    case RobotState::ALERT:
      setColor(0, 200, 0); // solid green
      break;
    case RobotState::DANGER: {
      // fast red flash
      bool on = ((now / 150) % 2) == 0;
      setColor(on ? 255 : 20, 0, 0);
      break;
    }
  }
}

void updateBuzzer(unsigned long now, unsigned long intervalMs) {
  if (now - s_lastBuzzMs >= intervalMs) {
    s_lastBuzzMs = now;
    s_buzzerOn = !s_buzzerOn;
    digitalWrite(PIN_BUZZER, s_buzzerOn ? HIGH : LOW);
  }
}

void startChirp(unsigned long now) {
  digitalWrite(PIN_BUZZER, HIGH);
  s_buzzerOn = true;
  s_chirpStartMs = now;
  s_chirping = true;
}

bool tickChirp(unsigned long now) {
  if (!s_chirping) return false;
  if (now - s_chirpStartMs >= 80) {
    digitalWrite(PIN_BUZZER, LOW);
    s_buzzerOn = false;
    s_chirping = false;
    return false;
  }
  return true;
}

void buzzerOff() {
  digitalWrite(PIN_BUZZER, LOW);
  s_buzzerOn = false;
  s_chirping = false;
}