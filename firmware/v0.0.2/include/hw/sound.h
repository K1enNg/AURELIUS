#pragma once

namespace sound {
void begin();
void beep(int ms = 40);   // silent while muted
bool isMuted();
void toggleMute();
}
