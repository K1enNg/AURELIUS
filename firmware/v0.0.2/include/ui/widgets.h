#pragma once
#include <Arduino.h>

// Small drawing helpers shared by all screens (320 x 480 portrait).
namespace widgets {
void drawHeader(const char* title);
// One or two centred lines in the hint area at the bottom
void drawHint(const char* l1, const char* l2 = nullptr);
// Two-line info card: small grey label, larger white value underneath
void drawInfoRow(int y, const char* label, const String& value);
String clip(String s, size_t n);
}
