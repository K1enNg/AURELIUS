#pragma once
#include <Arduino.h>

namespace widgets {
void drawHeader(const char* title);
void drawHint(const char* l1, const char* l2 = nullptr);
void drawInfoRow(int y, const char* label, const String& value);
String clip(String s, size_t n);
}
