#pragma once
#include <Arduino.h>
#include <U8g2lib.h>
#include "sensors.h"

enum class Mood : uint8_t { Sleeping, Idle, Happy, Curious, Surprised, Love, Count };

// Digital pet: decides a mood from the sensors and draws its face on the OLED.
class Pet {
 public:
  void begin();
  void update(const SensorState& s, uint32_t now);  // pick the mood
  void render(U8G2& oled, uint32_t now);            // draw the face (uses y = 0..51)

  Mood mood() const { return _mood; }
  const char* moodName() const;
  bool moodChanged();  // true once after every mood change

  // Demo mode: choose the expression by hand (ignores the sensors)
  bool demo() const { return _demo; }
  void setDemo(bool on);
  void demoStep(int dir);

 private:
  void setMood(Mood m, uint32_t now);

  Mood _mood = Mood::Idle;
  bool _changed = false;
  bool _demo = false;
  uint32_t _moodSince = 0;
  uint32_t _lastActivity = 0;
  uint32_t _closeSince = 0;

  // animation state
  uint32_t _nextBlink = 0, _blinkUntil = 0, _nextGlance = 0;
  float _gx = 0, _gy = 0, _tgx = 0, _tgy = 0;
};