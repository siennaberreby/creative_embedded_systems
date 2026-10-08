/* Lab 1: Neon Garden
   Original TTGO T-Display (ESP32, 135 x 240), TFT_eSPI Setup25.
   Randomly positioned, colored blooms expand across a star field.
   Each bloom gets new parameters, so the animation is not a fixed cycle.
*/
#include <Arduino.h>
#include <TFT_eSPI.h>
#include <esp_system.h>

TFT_eSPI tft;
TFT_eSprite canvas(&tft);

const int WIDTH = 240;
const int HEIGHT = 135;
const int BLOOM_COUNT = 6;
const uint16_t PALETTE[] = {
  TFT_CYAN, TFT_MAGENTA, TFT_YELLOW, TFT_GREEN, TFT_ORANGE, TFT_PINK
};
const int COLOR_COUNT = sizeof(PALETTE) / sizeof(PALETTE[0]);

struct Bloom {
  int x, y;
  float radius, speed;
  int limit, colorIndex;
};
Bloom blooms[BLOOM_COUNT];
int starX[28], starY[28];
bool displayReady = false;
uint32_t previousFrame = 0;

void resetBloom(int index) {
  Bloom &b = blooms[index];
  b.x = random(20, WIDTH - 20);
  b.y = random(35, HEIGHT - 20);
  b.radius = 1;
  b.speed = random(35, 85) / 100.0f;
  b.limit = random(24, 49);
  b.colorIndex = random(COLOR_COUNT);
}

void setup() {
  tft.init();
  tft.setRotation(1);
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);
  tft.fillScreen(TFT_BLACK);
  randomSeed(esp_random());

  // Draw offscreen, then send the complete frame to prevent flicker.
  canvas.setColorDepth(16);
  if (canvas.createSprite(WIDTH, HEIGHT) == nullptr) {
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setCursor(8, 20);
    tft.println("Display buffer failed.");
    tft.println("Press reset to retry.");
    return;
  }
  displayReady = true;
  canvas.setTextFont(1);
  canvas.setTextWrap(false);
  for (int i = 0; i < 28; ++i) {
    starX[i] = random(WIDTH);
    starY[i] = random(25, HEIGHT - 12);
  }
  for (int i = 0; i < BLOOM_COUNT; ++i) {
    resetBloom(i);
    blooms[i].radius = random(1, blooms[i].limit);
  }
}

void loop() {
  if (!displayReady) {
    delay(100);
    return;
  }
  uint32_t now = millis();
  if (now - previousFrame < 33) {
    delay(1);
    return;
  }
  previousFrame = now;
  canvas.fillSprite(TFT_BLACK);

  for (int i = 0; i < 28; ++i) {
    canvas.drawPixel(starX[i], starY[i], TFT_DARKGREY);
  }

  for (int i = 0; i < BLOOM_COUNT; ++i) {
    Bloom &b = blooms[i];
    b.radius += b.speed;
    if (b.radius > b.limit) resetBloom(i);
    int outerRadius = (int)b.radius;
    // Alternating palette colors in each group of concentric circles.
    for (int ring = 0; ring < 4; ++ring) {
      int r = outerRadius - ring * 7;
      if (r > 0) {
        canvas.drawCircle(b.x, b.y, r,
                          PALETTE[(b.colorIndex + ring) % COLOR_COUNT]);
      }
    }
    canvas.fillCircle(b.x, b.y, 2, TFT_WHITE);
  }

  // Opaque bands keep text readable when circles cross the edges.
  canvas.fillRect(0, 0, WIDTH, 25, TFT_BLACK);
  canvas.setTextSize(2);
  canvas.setTextColor(TFT_CYAN, TFT_BLACK);
  canvas.setCursor(6, 4);
  canvas.print("NEON GARDEN");
  canvas.drawFastHLine(6, 23, WIDTH - 12, TFT_DARKCYAN);
  canvas.fillRect(0, HEIGHT - 12, WIDTH, 12, TFT_BLACK);
  canvas.setTextSize(1);
  canvas.setTextColor(TFT_WHITE, TFT_BLACK);
  canvas.setCursor(6, HEIGHT - 9);
  canvas.print("LAB 1 / randomly growing light");
  canvas.pushSprite(0, 0);
}
