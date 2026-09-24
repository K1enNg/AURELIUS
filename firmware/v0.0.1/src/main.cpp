#include <Arduino.h>
#include "config.h"
#include "sensor.h"
#include "button.h"
#include "output.h"

Mood moods[] = {
  {"Calm",    15.0f},
  {"Nervous", 30.0f},
  {"Jumpy",   50.0f},
};
const uint8_t moodCount = sizeof(moods) / sizeof(moods[0]);

RobotState state = RobotState::IDLE;
uint8_t moodIndex = 0;
bool muted = false;

unsigned long lastUltrasonicMs = 0;
unsigned long stateStartMs = 0;

float lastDistanceCm = 400.0f;
bool motionDetected = false;

Button btn1 = makeButton(PIN_BTN1);
Button btn2 = makeButton(PIN_BTN2);

void setup() {
  Serial.begin(115200);

  sensorsInit();
  buttonInit(btn1);
  buttonInit(btn2);
  outputsInit();

  Serial.println("Interactive robot booted.");
}

void loop() {
  unsigned long now = millis();

  if (now - lastUltrasonicMs >= ULTRASONIC_INTERVAL_MS) {
    lastUltrasonicMs = now;
    lastDistanceCm = readDistanceCm();
  }
  motionDetected = readMotion();

  if (updateButton(btn1)) {
    muted = !muted;
    Serial.printf("Buzzer %s\n", muted ? "muted" : "unmuted");
    if (muted) buzzerOff();
  }
  if (updateButton(btn2)) {
    moodIndex = (moodIndex + 1) % moodCount;
    Serial.printf("Mood -> %s (danger threshold %.0f cm)\n",
                  moods[moodIndex].name, moods[moodIndex].dangerCm);
  }

  RobotState prevState = state;
  if (lastDistanceCm > 0 && lastDistanceCm <= moods[moodIndex].dangerCm) {
    state = RobotState::DANGER;
  } else if (motionDetected) {
    state = RobotState::ALERT;
  } else {
    state = RobotState::IDLE;
  }

  bool justEnteredState = (state != prevState);
  if (justEnteredState) {
    stateStartMs = now;
    Serial.printf("State -> %s (distance=%.1fcm, motion=%d)\n",
                  state == RobotState::DANGER ? "DANGER" :
                  state == RobotState::ALERT  ? "ALERT"  : "IDLE",
                  lastDistanceCm, motionDetected);
  }

  updateLedForState(state, now, stateStartMs);

  if (!muted) {
    switch (state) {
      case RobotState::DANGER: {
        // beep faster as the object gets closer; map distance to interval
        unsigned long interval = map((long)lastDistanceCm, 0, (long)moods[moodIndex].dangerCm, 60, 300);
        interval = constrain(interval, 60UL, 300UL);
        updateBuzzer(now, interval);
        break;
      }
      case RobotState::ALERT: {
        if (justEnteredState) {
          startChirp(now);
        } else {
          tickChirp(now);
        }
        break;
      }
      case RobotState::IDLE:
      default:
        buzzerOff();
        break;
    }
  }
}