#include <SoftwareSerial.h>
#include<SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <XPT2046_Touchscreen.h>

#define TFT_CS D2
#define TFT_DC D4
#define TFT_RST -1
#define TS_CS D1
#define LED_PIN D0

SPISettings settings(16000000, MSBFIRST, SPI_MODE0);
SoftwareSerial mySerial(D3,D8);
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
XPT2046_Touchscreen ts(TS_CS);

unsigned long previousMillis = -5000,previous_time = 0,elapsed_time = 0,start_time,current_time;
const long interval = 5000,intervalsec=1000; 
unsigned long currentMillis;
bool v2v=false;
bool v2g=false;
bool clear=false;
float dcvoltage,dccurrent,dcenergy,dccharge;
char time_string[9];


void setup() 
{
  start_time=millis();
  Serial.begin(9600);
  mySerial.begin(9600);
  tft.begin();
  ts.begin();
  tft.setRotation(1);
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextSize(2);
  tft.setCursor(0, 0);
  tft.setTextColor(ILI9341_WHITE);
  drawSwitchv2v(30, 30, v2v);
  drawSwitchv2g(190, 30, v2g);
  defaulttext();
}

void loop()
 {
   currentMillis = millis();
   getdata();
  TS_Point p = ts.getPoint();
  if (ts.touched()) 
  {
    // touchscreen is being touched
    if (p.x >= 2368 && p.x <= 3771 && p.y >=2011  && p.y <= 3380&&v2g==false)
     {
      // switch is being toggled
      v2v=!v2v;
      drawSwitchv2v(30, 30, v2v);
      delay(50); // debounce delay
      v2vtext();
    }
    else
    if (p.x >= 791 && p.x <= 1953 && p.y >=2063  && p.y <= 3120&&v2v==false)
     {
      // switch is being toggled
      v2g=!v2g;
      drawSwitchv2g(190, 30, v2g);
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

void getdata()
{
if (mySerial.available() > 0) {
    String data = mySerial.readStringUntil('\n');
    Serial.println(data);
    
    // Extract voltage, energy, and charge values from the data
    dccurrent = getValue(data, 'C');
    dcvoltage = getValue(data, 'V');
    dcenergy = getValue(data, 'W');
    dccharge = getValue(data, 'A');
    
    Serial.print("Current:");
    Serial.print(dccurrent);
    Serial.print("Voltage: ");
    Serial.print(dcvoltage);
    Serial.print(" V, Energy: ");
    Serial.print(dcenergy);
    Serial.print(" Wh, Charge: ");
    Serial.print(dccharge);
    Serial.println(" Ah");
  }
}

float getValue(String data, char code) 
{
  int index = data.indexOf(code) + 1;
  int endIndex = data.indexOf('D', index);
  String valueStr = data.substring(index, endIndex);
  float value = valueStr.toFloat();
  return value;
}

void defaulttext()
{
  start_time=millis();
  if (currentMillis - previousMillis >= interval) 
  {
    previousMillis = currentMillis;
  if(clear==true)
  {
  tft.setCursor(240, 220);// power transmitted
  tft.fillRect(240, 220, 80, 20, ILI9341_BLACK);
  tft.setCursor(220, 180);// state
  tft.fillRect(220, 180, 50, 20, ILI9341_BLACK);
  tft.setCursor(200, 200);
  tft.fillRect(200, 200, 130, 20, ILI9341_BLACK);
  }

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
  tft.println("BATTERY VOLTAGE :");
  tft.setCursor(240, 140);
  tft.fillRect(240, 140, 50, 20, ILI9341_BLACK);
  tft.println(dcvoltage);
  tft.setCursor(300, 140);
  tft.println("V");
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
  tft.println("POWER TRANSMITTED  0W");
  clear=false;
}
}

void v2vtext()
{
  current_time = millis();
  if (current_time - previous_time >= 1000)
  {
     previous_time = current_time;
    elapsed_time = current_time - start_time;
    int hours = elapsed_time / 3600000;
    int minutes = (elapsed_time / 60000) % 60;
    int seconds = (elapsed_time / 1000) % 60;
    snprintf(time_string, sizeof(time_string), "%02d:%02d:%02d", hours, minutes, seconds);
    tft.setCursor(200, 200);
    tft.fillRect(200, 200, 130, 20, ILI9341_BLACK);
    tft.println(time_string);
  }
  
  if (currentMillis - previousMillis >= interval) 
  {
    previousMillis = currentMillis;
  drawSwitchv2v(30, 30, v2v);
  drawSwitchv2g(190, 30, v2g);
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
  tft.println("BATTERY VOLTAGE :");
  tft.setCursor(240, 140);
  tft.fillRect(240, 140, 50, 20, ILI9341_BLACK);
  tft.println(dcvoltage);
  tft.setCursor(300, 140);
  tft.println("V");
  tft.setTextSize(2);
  tft.setCursor(30, 160);
  tft.setTextColor(ILI9341_WHITE);
  tft.println("SYSTEM TEMP : 25 C");
  tft.setTextSize(2);
  tft.setCursor(30, 180);
  tft.setTextColor(ILI9341_WHITE);
  tft.println("SYSTEM STATUS :");
  tft.setCursor(220, 180);
  tft.fillRect(220, 180, 50, 20, ILI9341_BLACK);
  tft.println("V2V");
  tft.setTextSize(2);
  tft.setCursor(30, 200);
  tft.setTextColor(ILI9341_WHITE);
  tft.println("TIME ELAPSED :");
  tft.setTextSize(2);
  tft.setCursor(30, 220);
  tft.setTextColor(ILI9341_WHITE);
  tft.println("POWER TRANSMITTED:");
  tft.setCursor(245, 220);
  tft.fillRect(245, 220, 80, 20, ILI9341_BLACK);
  tft.println(dcenergy);
  tft.setCursor(300, 220);
  tft.println("W");
  clear=true;
  //delay(5000);
}
}
void v2gtext()
{
  tft.fillScreen(ILI9341_BLACK);
  drawSwitchv2v(30, 30, v2v);
  drawSwitchv2g(190, 30, v2g);
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
