#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <XPT2046_Touchscreen.h>

#define TFT_CS D2
#define TFT_DC D4
#define TFT_RST -1
#define TS_CS D1
#define LED_PIN D0


Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
XPT2046_Touchscreen ts(TS_CS);
unsigned long previousMillis = 0;
const long interval = 2000; 
unsigned long currentMillis;
bool ledState = false;
bool ledState1 = false;
bool v2v=false;
bool v2g=false;


void setup() {
  pinMode(LED_PIN, OUTPUT);
  tft.begin();
  ts.begin();
  tft.setRotation(1); // adjust rotation as necessary
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextSize(2);
  tft.setCursor(0, 0);
  tft.setTextColor(ILI9341_WHITE);
  //tft.println("Touch the switch to turn the LED on/off");
  drawSwitchv2v(30, 30, ledState);
  drawSwitchv2g(190, 30, ledState1);
  defaulttext();
}

void loop()
 {
   currentMillis = millis();
  TS_Point p = ts.getPoint();
  if (ts.touched()) 
  {
    // touchscreen is being touched
    if (p.x >= 2368 && p.x <= 3771 && p.y >=2011  && p.y <= 3380&&v2g==false)
     {
      // switch is being toggled
      v2v=!v2v;
      ledState = !ledState;
      digitalWrite(LED_PIN, ledState ? HIGH : LOW);
      drawSwitchv2v(30, 30, ledState);
      delay(50); // debounce delay
      v2vtext();
    }
    else
    if (p.x >= 791 && p.x <= 1953 && p.y >=2063  && p.y <= 3120&&v2v==false)
     {
      // switch is being toggled
      v2g=!v2g;
      ledState = !ledState;
      digitalWrite(LED_PIN, ledState ? HIGH : LOW);
      drawSwitchv2g(190, 30, ledState);
      delay(50); // debounce delay
     defaulttext();
    }
  }
  else
  if(v2v==true)
  {
    v2vtext();
  }
  else
  if(v2v==false)
  {
    defaulttext();
  }
}


void drawSwitchv2v(int x, int y, bool state) {
  tft.fillRect(x - 30, y - 10, 150, 100, state ? ILI9341_GREEN : ILI9341_RED);
  tft.drawRect(x - 30, y - 10, 150, 100, ILI9341_WHITE);
  tft.fillRect(x - 20, y - 8, 130, 96, state ? ILI9341_BLACK : ILI9341_BLACK);
}

void drawSwitchv2g(int x, int y, bool state) {
  tft.fillRect(x - 30, y - 10, 150, 100, state ? ILI9341_GREEN : ILI9341_RED);
  tft.drawRect(x - 30, y - 10, 150, 100, ILI9341_WHITE);
  tft.fillRect(x - 20, y - 8, 130, 96, state ? ILI9341_BLACK : ILI9341_BLACK);
}

void defaulttext()
{
  tft.fillScreen(ILI9341_BLACK);
  drawSwitchv2v(30, 30, ledState);
  drawSwitchv2g(190, 30, ledState1);
  tft.setTextSize(5);
  tft.setCursor(30, 50);
  tft.setTextColor(ILI9341_WHITE);
  tft.println("V2V");
  tft.setTextSize(5);
  tft.setCursor(190, 50);
  tft.setTextColor(ILI9341_WHITE);
  tft.println("V2G");
  tft.setTextSize(2);
  tft.setCursor(30, 140);
  tft.setTextColor(ILI9341_WHITE);
  tft.println("BATTERY VOLTAGE :72V");
  tft.setTextSize(2);
  tft.setCursor(30, 160);
  tft.setTextColor(ILI9341_WHITE);
  tft.println("SYSTEM TEMP : 25 C");
  tft.setTextSize(2);
  tft.setCursor(30, 180);
  tft.setTextColor(ILI9341_WHITE);
  tft.println("SYSTEM STATUS : OFF");
  tft.setTextSize(2);
  tft.setCursor(30, 200);
  tft.setTextColor(ILI9341_WHITE);
  tft.println("TIME ELAPSED : 00:00:00");
  tft.setTextSize(2);
  tft.setCursor(30, 220);
  tft.setTextColor(ILI9341_WHITE);
  tft.println("POWER TRANSMITTED : 0W");
}

void v2vtext()
{
  
  if (currentMillis - previousMillis >= interval) 
  {
    previousMillis = currentMillis;
  tft.fillScreen(ILI9341_BLACK);
  drawSwitchv2v(30, 30, ledState);
  drawSwitchv2g(190, 30, ledState1);
  tft.setTextSize(5);
  tft.setCursor(30, 50);
  tft.setTextColor(ILI9341_WHITE);
  tft.println("V2V");
  tft.setTextSize(5);
  tft.setCursor(190, 50);
  tft.setTextColor(ILI9341_WHITE);
  tft.println("V2G");
  tft.setTextSize(2);
  tft.setCursor(30, 140);
  tft.setTextColor(ILI9341_WHITE);
  tft.println("BATTERY VOLTAGE :72V");
  tft.setTextSize(2);
  tft.setCursor(30, 160);
  tft.setTextColor(ILI9341_WHITE);
  tft.println("SYSTEM TEMP : 25 C");
  tft.setTextSize(2);
  tft.setCursor(30, 180);
  tft.setTextColor(ILI9341_WHITE);
  tft.println("SYSTEM STATUS : V2V");
  tft.setTextSize(2);
  tft.setCursor(30, 200);
  tft.setTextColor(ILI9341_WHITE);
  tft.println("TIME ELAPSED : 00:00:00");
  tft.setTextSize(2);
  tft.setCursor(30, 220);
  tft.setTextColor(ILI9341_WHITE);
  tft.println("POWER TRANSMITTED : 0W");
  //delay(5000);
}
}
void v2gtext()
{
  tft.fillScreen(ILI9341_BLACK);
  drawSwitchv2v(30, 30, ledState);
  drawSwitchv2g(190, 30, ledState1);
  tft.setTextSize(5);
  tft.setCursor(30, 50);
  tft.setTextColor(ILI9341_WHITE);
  tft.println("V2V");
  tft.setTextSize(5);
  tft.setCursor(190, 50);
  tft.setTextColor(ILI9341_WHITE);
  tft.println("V2G");
  tft.setTextSize(2);
  tft.setCursor(30, 140);
  tft.setTextColor(ILI9341_WHITE);
  tft.println("BATTERY VOLTAGE :72V");
  tft.setTextSize(2);
  tft.setCursor(30, 160);
  tft.setTextColor(ILI9341_WHITE);
  tft.println("SYSTEM TEMP : 25 C");
  tft.setTextSize(2);
  tft.setCursor(30, 180);
  tft.setTextColor(ILI9341_WHITE);
  tft.println("SYSTEM STATUS : V2G");
  tft.setTextSize(2);
  tft.setCursor(30, 200);
  tft.setTextColor(ILI9341_WHITE);
  tft.println("TIME ELAPSED : 00:00:00");
  tft.setTextSize(2);
  tft.setCursor(30, 220);
  tft.setTextColor(ILI9341_WHITE);
  tft.println("POWER TRANSMITTED : 0W");
  delay(5000);
}
