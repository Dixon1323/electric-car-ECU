#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
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

const char *ssid     = "Chathamkottu";
const char *password = "12345678";
bool ledState = false;
const long utcOffsetInSeconds = 19800;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

void setup(){
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  tft.begin();
  ts.begin();
  tft.setRotation(1);
  WiFi.begin(ssid, password);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
    tft.fillScreen(ILI9341_BLACK);
  tft.setTextSize(2);
  tft.setCursor(0, 0);
  tft.setTextColor(ILI9341_WHITE);
  tft.println(".");
  }
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextSize(2);
  tft.setCursor(0, 0);
  tft.setTextColor(ILI9341_WHITE);
  tft.println("wifi connected");
  delay(2000);
  tft.fillScreen(ILI9341_BLACK);
  drawSwitch(120, 120, ledState);
  timeClient.begin();
}

void loop() {
  timeClient.update();
   // tft.fillScreen(ILI9341_BLACK);
   tft.fillRect(0, 0, 300, 50, ILI9341_BLACK);
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

  tft.setTextSize(2);
  tft.setCursor(200, 0);
  tft.setTextColor(ILI9341_WHITE);
  tft.println(timeClient.getFormattedTime());
  delay(1000);

  /*Serial.print(daysOfTheWeek[timeClient.getDay()]);
  Serial.print(", ");
  Serial.print(timeClient.getHours());
  Serial.print(":");
  Serial.print(timeClient.getMinutes());
  Serial.print(":");
  Serial.println(timeClient.getSeconds());
  //Serial.println(timeClient.getFormattedTime());*/

}

void drawSwitch(int x, int y, bool state) {
  tft.fillRect(x - 30, y - 10, 60, 20, state ? ILI9341_GREEN : ILI9341_RED);
  tft.drawRect(x - 30, y - 10, 60, 20, ILI9341_WHITE);
  tft.fillRect(x - 20, y - 8, 40, 16, state ? ILI9341_WHITE : ILI9341_BLACK);
}