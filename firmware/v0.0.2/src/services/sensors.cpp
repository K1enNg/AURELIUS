#include "services/sensors.h"

namespace {
constexpr uint32_t PIR_WARMUP_MS    = 30000;
constexpr uint32_t PING_INTERVAL_MS = 100;
constexpr uint32_t ECHO_TIMEOUT_US  = 8000;  
constexpr int      MIN_CM           = 2;
constexpr int      MAX_CM           = 120;
}

void Sensors::begin() {
  pinMode(_trig, OUTPUT);
  digitalWrite(_trig, LOW);
  pinMode(_echo, INPUT);
  pinMode(_pir, INPUT);
  _bootMs = millis();
}

int Sensors::readDistanceCm() {
  digitalWrite(_trig, LOW);
  delayMicroseconds(2);
  digitalWrite(_trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(_trig, LOW);

  unsigned long us = pulseIn(_echo, HIGH, ECHO_TIMEOUT_US);
  if (us == 0) return -1;      // no echo within range
  int cm = (int)(us / 58);     // round-trip time -> centimetres
  if (cm < MIN_CM || cm > MAX_CM) return -1;
  return cm;
}

void Sensors::update() {
  uint32_t now = millis();

  // PIR warm-up countdown
  uint32_t up = now - _bootMs;
  _s.warmingUp = up < PIR_WARMUP_MS;
  _s.warmupLeftS = _s.warmingUp ? (PIR_WARMUP_MS - up + 999) / 1000 : 0;

  // ultrasonic: ping on a timer, smooth the result
  if (now - _lastPing >= PING_INTERVAL_MS) {
    _lastPing = now;
    int cm = readDistanceCm();
    if (cm >= 0) {
      _miss = 0;
      if (_s.distCm < 0 || abs(cm - _s.distCm) > 15) _s.distCm = cm;       // first reading or big jump
      else _s.distCm = (_s.distCm * 6 + cm * 4) / 10;      // smooth small changes
    } else if (++_miss >= 3) {
      _s.distCm = -1;  // lost the object
    }
  }

  // PIR
  bool pir = !_s.warmingUp && digitalRead(_pir) == HIGH;
  if (pir && !_s.motion) _s.motionEvents++;
  if (pir) _lastMotion = now;
  _s.motion = pir;
  _s.sinceMotionMs = _lastMotion ? now - _lastMotion : UINT32_MAX;
}