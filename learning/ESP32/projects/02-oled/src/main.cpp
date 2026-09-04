#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

unsigned long flashCounter = 0;

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.display();
}

// Render text at a given (x, y) coordinate
void drawTextLine(const char* text, int x, int y, int textSize) {
  display.setTextSize(textSize);
  display.setTextColor(WHITE);
  display.setCursor(x, y);
  display.println(text);
}

void drawFlashTransition() {
  display.fillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);
  display.display();
  delay(100);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(10, 25);
  display.println("Hello World!");
  display.print(F(R"(
  /^_^\
 ( o.o )
  > ^ <
 SMILE :)
)"));
  display.display();
  delay(10000);
}

void loop() {
  display.clearDisplay();

  // Primary text line centered on screen
  drawTextLine("SYSTEM ACTIVE", 24, 28, 1);

  display.display();
  delay(50);

  // Flash transition every 10 seconds
  if (millis() - flashCounter > 10000) {
    drawFlashTransition();
    flashCounter = millis();
  }
}