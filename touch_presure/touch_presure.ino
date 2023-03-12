#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <XPT2046_Touchscreen.h>
#include <SPI.h>

#define TFT_CS D2
#define TFT_DC D4
#define TFT_RST D8
#define TS_CS D1

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
XPT2046_Touchscreen ts(TS_CS);

void setup() {
  tft.begin();
  ts.begin();
  tft.setRotation(1); // adjust rotation as necessary
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextSize(2);
  tft.setCursor(0, 0);
  tft.setTextColor(ILI9341_WHITE);
  tft.println("Touch the screen!");
}

void loop() {
  TS_Point p = ts.getPoint();
  if (ts.touched()) {
    // touchscreen is being touched
    tft.fillScreen(ILI9341_BLACK);
    tft.setCursor(0, 0);
    tft.print("X: ");
    tft.println(p.x);
    tft.print("Y: ");
    tft.println(p.y);
    tft.print("Pressure: ");
    tft.println(p.z);
  }
}
