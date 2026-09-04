#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define PIR_PIN        4

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

bool motionDetected = false;
unsigned long lastMotionTime = 0;
const unsigned long displayHoldMs = 5000;

void setup() {
  Serial.begin(115200);
  pinMode(PIR_PIN, INPUT);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 allocation failed");
    while(true) delay(10);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("PIR + OLED ready");
  display.display();
  delay(1000);
}

void loop() {
  int pirState = digitalRead(PIR_PIN);

  if (pirState == HIGH) {
    motionDetected = true;
    lastMotionTime = millis();
  }

  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);

  if (motionDetected && (millis() - lastMotionTime < displayHoldMs)) {
    display.setTextSize(2);
    display.println("Motion");
    display.println("Detected");
  } else {
    motionDetected = false;
    display.println("Status: idle");
    display.println("Waiting for");
    display.println("motion...");
  }

  display.display();
  delay(100);
}