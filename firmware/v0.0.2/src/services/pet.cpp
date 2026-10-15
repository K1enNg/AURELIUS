#include "services/pet.h"

namespace {
// ---- behaviour tuning ----
constexpr int      CLOSE_CM       = 15;     // hand this close = surprised, then love
constexpr int      CURIOUS_CM     = 60;     // something within this range = curious
constexpr uint32_t LOVE_AFTER_MS  = 1500;   // stay close this long -> love
constexpr uint32_t HAPPY_HOLD_MS  = 4000;   // stay happy this long after motion
constexpr uint32_t SLEEP_AFTER_MS = 60000;  // nobody around this long -> sleep
constexpr uint32_t MIN_HOLD_MS    = 700;    // minimum time before changing mood

// ---- face geometry (128 x 64 OLED, footer uses the bottom rows) ----
constexpr int EX1 = 38, EX2 = 90, EY = 22;

const char* const MOOD_NAMES[] = {"Sleeping", "Idle", "Happy", "Curious", "Surprised", "In love"};

// ----- drawing helpers -----
void normalEye(U8G2& o, int cx, int cy, bool blink, int gx, int gy) {
  o.setDrawColor(1);
  if (blink) { o.drawRBox(cx - 11, cy - 1, 22, 4, 2); return; }
  o.drawRBox(cx - 11, cy - 13, 22, 26, 7);
  o.setDrawColor(0);
  o.drawDisc(cx + gx, cy + gy, 6);
  o.setDrawColor(1);
  o.drawDisc(cx + gx - 2, cy + gy - 3, 2);  // highlight
}

void happyEye(U8G2& o, int cx, int cy) {  // ^ shape
  o.setDrawColor(1);
  for (int r = 9; r <= 11; r++)
    o.drawCircle(cx, cy + 4, r, U8G2_DRAW_UPPER_LEFT | U8G2_DRAW_UPPER_RIGHT);
}

void sleepEye(U8G2& o, int cx, int cy) {  // closed, peaceful curve
  o.setDrawColor(1);
  for (int r = 8; r <= 9; r++)
    o.drawCircle(cx, cy - 3, r, U8G2_DRAW_LOWER_LEFT | U8G2_DRAW_LOWER_RIGHT);
}

void bigEye(U8G2& o, int cx, int cy, int gx, int gy) {
  o.setDrawColor(1);
  o.drawDisc(cx, cy, 12);
  o.setDrawColor(0);
  o.drawDisc(cx + gx / 2, cy + gy / 2, 4);
  o.setDrawColor(1);
  o.drawDisc(cx + gx / 2 - 1, cy + gy / 2 - 2, 1);
}

void heart(U8G2& o, int cx, int cy, int r) {
  o.setDrawColor(1);
  o.drawDisc(cx - r, cy - r / 2, r);
  o.drawDisc(cx + r, cy - r / 2, r);
  o.drawTriangle(cx - 2 * r + 1, cy - r / 2 + 2, cx + 2 * r - 1, cy - r / 2 + 2, cx, cy + 2 * r);
}

void smile(U8G2& o, int cx, int cy, int r) {
  o.setDrawColor(1);
  o.drawCircle(cx, cy, r,     U8G2_DRAW_LOWER_LEFT | U8G2_DRAW_LOWER_RIGHT);
  o.drawCircle(cx, cy, r + 1, U8G2_DRAW_LOWER_LEFT | U8G2_DRAW_LOWER_RIGHT);
}

void openSmile(U8G2& o, int cx, int cy, int r) {  // filled "D" mouth
  o.setDrawColor(1);
  o.drawDisc(cx, cy, r, U8G2_DRAW_LOWER_LEFT | U8G2_DRAW_LOWER_RIGHT);
}

void cheeks(U8G2& o, int yo) {
  o.setDrawColor(1);
  o.drawDisc(14, 40 + yo, 2);
  o.drawDisc(114, 40 + yo, 2);
}
}  // namespace

void Pet::begin() {
  uint32_t now = millis();
  _moodSince = now;
  _lastActivity = now;
  _nextBlink = now + 3000;
  _nextGlance = now + 2000;
}

const char* Pet::moodName() const { return MOOD_NAMES[(int)_mood]; }

bool Pet::moodChanged() {
  bool c = _changed;
  _changed = false;
  return c;
}

void Pet::setMood(Mood m, uint32_t now) {
  _mood = m;
  _moodSince = now;
  _changed = true;
}

void Pet::setDemo(bool on) {
  _demo = on;
  _moodSince = millis();
}

void Pet::demoStep(int dir) {
  _demo = true;
  int n = (int)Mood::Count;
  int next = ((int)_mood + dir + n) % n;
  setMood((Mood)next, millis());
}

void Pet::update(const SensorState& s, uint32_t now) {
  if (_demo) return;

  bool close  = s.distCm >= 0 && s.distCm <= CLOSE_CM;
  bool near   = s.distCm >= 0 && s.distCm <= CURIOUS_CM;
  bool moving = s.motion || s.sinceMotionMs < HAPPY_HOLD_MS;

  if (close) { if (!_closeSince) _closeSince = now; } else _closeSince = 0;
  if (s.motion || near) _lastActivity = now;

  Mood target;
  if (close)                                      target = (now - _closeSince < LOVE_AFTER_MS) ? Mood::Surprised : Mood::Love;
  else if (moving)                                target = Mood::Happy;
  else if (near)                                  target = Mood::Curious;
  else if (now - _lastActivity > SLEEP_AFTER_MS)  target = Mood::Sleeping;
  else                                            target = Mood::Idle;

  if (target != _mood && now - _moodSince >= MIN_HOLD_MS) setMood(target, now);
}

void Pet::render(U8G2& o, uint32_t now) {
  // ---- animation: random glances and blinks ----
  if (now >= _nextGlance) {
    _tgx = random(-4, 5);
    _tgy = random(-3, 4);
    _nextGlance = now + random(1500, 4000);
  }
  _gx += (_tgx - _gx) * 0.25f;
  _gy += (_tgy - _gy) * 0.25f;
  if (now >= _nextBlink) {
    _blinkUntil = now + 140;
    _nextBlink = now + random(2500, 5500);
  }
  bool blink = now < _blinkUntil;
  int gx = (int)lroundf(_gx), gy = (int)lroundf(_gy);

  switch (_mood) {
    case Mood::Idle:
      normalEye(o, EX1, EY, blink, gx, gy);
      normalEye(o, EX2, EY, blink, gx, gy);
      smile(o, 64, 37, 6);
      break;

    case Mood::Happy: {
      int yo = (int)(2.0f * sinf(now / 110.0f));  // happy bounce
      happyEye(o, EX1, EY + yo);
      happyEye(o, EX2, EY + yo);
      openSmile(o, 64, 36 + yo, 9);
      cheeks(o, yo);
      break;
    }

    case Mood::Curious:
      normalEye(o, EX1, EY, blink, 3, -3);
      normalEye(o, EX2, EY, blink, 3, -3);
      o.setDrawColor(1);
      o.drawLine(EX1 - 9, EY - 17, EX1 + 9, EY - 21);  // raised eyebrow
      o.drawLine(EX2 - 9, EY - 17, EX2 + 9, EY - 17);
      o.drawCircle(68, 42, 3);
      break;

    case Mood::Surprised:
      bigEye(o, EX1, EY, gx, gy);
      bigEye(o, EX2, EY, gx, gy);
      o.setDrawColor(1);
      o.drawLine(EX1 - 8, EY - 17, EX1 + 8, EY - 17);
      o.drawLine(EX2 - 8, EY - 17, EX2 + 8, EY - 17);
      o.drawCircle(64, 43, 5);
      o.drawCircle(64, 43, 4);
      break;

    case Mood::Love: {
      int r = 5 + (int)((now / 220) % 2);  // heartbeat
      heart(o, EX1, EY - 2, r);
      heart(o, EX2, EY - 2, r);
      smile(o, 64, 36, 8);
      cheeks(o, 0);
      break;
    }

    case Mood::Sleeping: {
      sleepEye(o, EX1, EY);
      sleepEye(o, EX2, EY);
      int br = 1 + (int)(sinf(now / 500.0f) + 1.0f);  // breathing mouth
      o.setDrawColor(1);
      o.drawDisc(64, 40, br);
      int p = (int)((now / 250) % 8);
      o.setFont(u8g2_font_6x10_tr);
      o.drawStr(98, 30 - p, "z");
      o.drawStr(106, 22 - p / 2, "Z");
      break;
    }

    default:
      break;
  }
  o.setDrawColor(1);
}