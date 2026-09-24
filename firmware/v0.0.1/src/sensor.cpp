#include "sensor.h"
#include "config.h"

void sensorsInit() {
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  pinMode(PIN_PIR, INPUT);
}

float readDistanceCm() {
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  unsigned long durationUs = pulseIn(PIN_ECHO, HIGH, 25000UL);
  if (durationUs == 0) {
    return -1.0f; // no echo received (out of range)
  }
  // Speed of sound ~0.0343 cm/us, divide by 2 for round trip
  return (durationUs * 0.0343f) / 2.0f;
}

bool readMotion() {
  return digitalRead(PIN_PIR) == HIGH;
}