#include <WiFi.h>
#include <TFT_eSPI.h>
#include <TJpg_Decoder.h>
#include "id.h"

const char* SSID = WIFI_SSID;
const char* PASS = WIFI_PASS;
const char* HOST = STREAM_HOST;
const uint16_t PORT = 8080;

#define MAX_JPG 60000

TFT_eSPI tft;
WiFiClient client;
uint8_t* jpg;

bool tftOutput(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bmp) {
  if (y >= tft.height()) return false;
  tft.pushImage(x, y, w, h, bmp);
  return true;
}

bool readFrame(size_t &len) {
  static uint8_t rx[1024];
  static int rxLen = 0, rxPos = 0;
  static uint8_t prev = 0;
  bool inJpg = false;
  len = 0;
  uint32_t t0 = millis();

  while (client.connected() && millis() - t0 < 3000) {
    if (rxPos >= rxLen) {
      int avail = client.available();
      if (!avail) { delay(1); continue; }
      rxLen = client.read(rx, min(avail, (int)sizeof(rx)));
      rxPos = 0;
      if (rxLen <= 0) { rxLen = 0; continue; }
    }
    uint8_t b = rx[rxPos++];
    if (!inJpg) {
      if (prev == 0xFF && b == 0xD8) {
        jpg[0] = 0xFF; jpg[1] = 0xD8; len = 2; inJpg = true;
      }
    } else {
      if (len < MAX_JPG) jpg[len++] = b;
      else { inJpg = false; len = 0; }            // frame too big, skip
      if (inJpg && prev == 0xFF && b == 0xD9) { prev = 0; return true; }
    }
    prev = b;
  }
  return false;
}

void connectStream() {
  client.stop();
  if (client.connect(HOST, PORT)) {
    client.print(String("GET /stream HTTP/1.0\r\nHost: ") + HOST + "\r\n\r\n");
  } else {
    delay(1000);
  }
}

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("boot");

  jpg = (uint8_t*)malloc(MAX_JPG);

  tft.init();
  tft.setRotation(1);
  tft.setSwapBytes(true);

  //stage 1: display self-test
  tft.fillScreen(TFT_RED);   delay(400);
  tft.fillScreen(TFT_GREEN); delay(400);
  tft.fillScreen(TFT_BLUE);  delay(400);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString("Display OK", 10, 10, 4);

  TJpgDec.setJpgScale(1);
  TJpgDec.setCallback(tftOutput);

  //stage 2: WiFi
  tft.drawString("Connecting WiFi...", 10, 50, 4);
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASS);
  while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }
  Serial.println(WiFi.localIP());
  tft.drawString("WiFi OK: " + WiFi.localIP().toString(), 10, 90, 4);
}

void loop() {
  //stage 3: stream
  if (!client.connected()) {
    Serial.println("connecting to stream...");
    tft.drawString("Connecting to PC...   ", 10, 130, 4);
    connectStream();
    return;
  }
  size_t len;
  if (readFrame(len)) {
    Serial.printf("frame %u bytes\n", (unsigned)len);
    TJpgDec.drawJpg(0, 0, jpg, len);
  } else {
    Serial.println("no frame (timeout)");
  }
}