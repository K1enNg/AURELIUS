#include <Arduino.h>
#include "hw/sound.h"
#include "config.h"

namespace sound {

static bool s_muted = false;

void begin() { pinMode(BUZZER, OUTPUT); }

void beep(int ms) {
  if (s_muted) return;
  digitalWrite(BUZZER, HIGH);
  delay(ms);
  digitalWrite(BUZZER, LOW);
}

bool isMuted()    { return s_muted; }
void toggleMute() { s_muted = !s_muted; }

}
