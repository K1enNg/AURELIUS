#include <Arduino.h>

constexpr uint8_t PIN_LED_HUNGER = 27;
constexpr uint8_t PIN_LED_HAPPY = 14;

void setup() {
    pinMode(PIN_LED_HUNGER, OUTPUT);
}

void loop() {
    digitalWrite(PIN_LED_HUNGER, HIGH);
    delay(500);
    digitalWrite(PIN_LED_HUNGER, LOW);
    delay(500);
    // digitalWrite(PIN_LED_HAPPY, HIGH);
    // delay(500);
    // digitalWrite(PIN_LED_HAPPY, LOW);
    // delay(500);
}