#pragma once
#include <Arduino.h>

// Snapshot of both sensors, refreshed by Sensors::update()
struct SensorState {
  int      distCm = -1;               // smoothed HC-SR04 distance, -1 = nothing in range
  bool     motion = false;            // HC-SR501 output currently HIGH
  uint32_t sinceMotionMs = UINT32_MAX; // time since PIR was last HIGH (UINT32_MAX = never)
  uint32_t motionEvents = 0;          // number of motion starts since boot
  bool     warmingUp = true;          // PIR needs ~30 s after power-up
  uint32_t warmupLeftS = 0;
};

class Sensors {
 public:
  Sensors(int trigPin, int echoPin, int pirPin)
      : _trig(trigPin), _echo(echoPin), _pir(pirPin) {}

  void begin();
  void update();  // call every loop; pings the ultrasonic sensor every 100 ms
  const SensorState& state() const { return _s; }

 private:
  int readDistanceCm();

  int _trig, _echo, _pir;
  SensorState _s;
  uint32_t _bootMs = 0;
  uint32_t _lastPing = 0;
  uint32_t _lastMotion = 0;
  int _miss = 0;
};