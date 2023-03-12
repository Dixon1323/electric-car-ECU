#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <XPT2046_Touchscreen.h>

#define TFT_CS D2
#define TFT_DC D4
#define TFT_RST D8
#define TS_CS D1
#define LED_PIN D0

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
XPT2046_Touchscreen ts(TS_CS);

bool ledState = false;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  tft.begin();
  ts.begin();
  tft.setRotation(1); // adjust rotation as necessary
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextSize(2);
  tft.setCursor(0, 0);
  tft.setTextColor(ILI9341_WHITE);
  tft.println("Touch the switch to turn the LED on/off");
  drawSwitch(120, 120, ledState);
}

void loop() {
  TS_Point p = ts.getPoint();
  if (ts.touched()) {
    // touchscreen is being touched
    if (p.x >= 2300 && p.x <= 2600 && p.y >= 1900. && p.y <= 2030) {
      // switch is being toggled
      ledState = !ledState;
      digitalWrite(LED_PIN, ledState ? HIGH : LOW);
      drawSwitch(120, 120, ledState);
      delay(50); // debounce delay
    }
  }
}

void drawSwitch(int x, int y, bool state) {
  tft.fillRect(x - 30, y - 10, 60, 20, state ? ILI9341_GREEN : ILI9341_RED);
  tft.drawRect(x - 30, y - 10, 60, 20, ILI9341_WHITE);
  tft.fillRect(x - 20, y - 8, 40, 16, state ? ILI9341_WHITE : ILI9341_BLACK);
}
