#include <SoftwareSerial.h>
#include<SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <XPT2046_Touchscreen.h>
#include <DHT.h>
#include <ESP8266WiFi.h>
#include<AdafruitIO_WiFi.h>

#define TFT_CS D2
#define TFT_DC D4
#define TFT_RST -1
#define TS_CS D1
#define LED_PIN D0
#define DHTPIN D8  
#define DHTTYPE DHT22 
#define IO_USERNAME "dixon1323"
#define IO_KEY "aio_FYHg27v3sa9zBEYC3aaMQ92BbeYx"
#define WLAN_SSID "Iphone 12"
#define WLAN_PASS "tubelight"

DHT dht(DHTPIN, DHTTYPE);
SPISettings settings(16000000, MSBFIRST, SPI_MODE0);
SoftwareSerial mySerial(D3,D0);
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
XPT2046_Touchscreen ts(TS_CS);
AdafruitIO_WiFi io(IO_USERNAME,IO_KEY,WLAN_SSID,WLAN_PASS);
AdafruitIO_Feed *voltage =io.feed("Ac voltage");
AdafruitIO_Feed *battery =io.feed("battery");
AdafruitIO_Feed *status =io.feed("status");
AdafruitIO_Feed *temperature =io.feed("temperature");
AdafruitIO_Feed *Energy =io.feed("Energy Transferred");
AdafruitIO_Feed *digital = io.feed("v2v");
AdafruitIO_Feed *digital2 = io.feed("v2g");


unsigned long previousMillis = -5000,previousMillis1 = -5000,previous_time = 0,elapsed_time = 0,start_time,current_time;
const long interval = 4000,intervalsec=1000,intervalupdate=30000; 
unsigned long currentMillis,currentMillis1;
bool v2v=false;
bool v2g=false;
bool clear=false;
float dcvoltage,dccurrent,dcenergy,dccharge,accurrent,acenergy,acpower,pf,temp,hum,templimit,batlimit,lat,lon;
int acvoltage,mode=0,energy;
char time_string[9];
String data,stats;


void setup() 
{
  start_time=millis();
  pinMode(A0,OUTPUT);
  analogWrite(A0,LOW);
  Serial.begin(9600);
  mySerial.begin(9600);
  tft.begin();
  ts.begin();
  tft.setRotation(1);
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextSize(2);
  tft.setCursor(60, 140);
  tft.setTextColor(ILI9341_WHITE);
  tft.println("Waiting to Connect");
  io.connect();
  digital->onMessage(handleMessage);
  digital2->onMessage(handleMessage2);
  while(io.status()<AIO_CONNECTED){
    Serial.print(".");
    delay(500);    
  }
  dht.begin();
  digital->get();
  digital2->get();
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
   io.run();
   currentMillis = millis();
   currentMillis1 = millis();
   getdcdata();
   getacdata();
   temper();
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
     v2gtext();
    }
}
else
  if(v2v==true)
  {
    v2vtext();
  }
  else
  if(v2g==true)
  {
    v2gtext();
  }
  else
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

void getdcdata()
{
if (mySerial.available() > 0)
 {
    data = mySerial.readStringUntil('\n');
   // Serial.println(data);
    
    // Extract voltage, energy, and charge values from the data
    dccurrent = getValue(data, 'C');
    dcvoltage = getValue(data, 'V');
    dcenergy = getValue(data, 'W');
    dccharge = getValue(data, 'A');
    
   /* Serial.print("Current:");
    Serial.print(dccurrent);
    Serial.print("Voltage: ");
    Serial.print(dcvoltage);
    Serial.print(" V, Energy: ");
    Serial.print(dcenergy);
    Serial.print(" Wh, Charge: ");
    Serial.print(dccharge);
    Serial.println(" Ah");*/
  }
}

void getacdata()
{
  if (Serial.available())
   {
    data = Serial.readStringUntil('\n');
   // Serial.println(data);
    accurrent = getValue(data, 'C');
    acvoltage = getValue(data, 'V');
    acpower = getValue(data, 'P');
    acenergy = getValue(data, 'E');
    pf=getValue(data,'F');
    lat=getValue(data,'L');
    lon=getValue(data,'l');
  // Serial.print("Current:");
    //Serial.print(lat);
    //Serial.print("Voltage: ");
    //Serial.print(lon);
    /* Serial.print(" V, Energy: ");
    Serial.print(acenergy);
    Serial.print(" kWh, power: ");
    Serial.print(acpower);
    Serial.print(" W");
    Serial.print("Power Factor:");
    Serial.println(pf);*/
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

float getValues(String data, char code) 
{
  int index = data.indexOf(code) + 1;
  int endIndex = data.indexOf('D', index);
  String valueStr = data.substring(index, endIndex);
  float value = valueStr.toFloat();
  return value;
}

void temper()
{
  temp = dht.readTemperature();
  hum = dht.readHumidity();
 /* Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.print(" °C, Humidity: ");
  Serial.print(hum);
  Serial.println(" %");*/
}

void defaulttext()
{
  
  stats="off";
  start_time=millis();
  mode=5;
  if (currentMillis - previousMillis >= interval) 
  {
    previousMillis = currentMillis;
  if(clear==true)
  {
  tft.setCursor(240, 220);// power transmitted
  tft.fillRect(240, 220, 80, 20, ILI9341_BLACK);
  tft.setCursor(200, 180);// state
  tft.fillRect(200, 180, 100, 20, ILI9341_BLACK);
  tft.setCursor(200, 200); //timer
  tft.fillRect(200, 200, 130, 20, ILI9341_BLACK);
  tft.setCursor(20, 220); //energy
  tft.fillRect(20, 220, 310, 20, ILI9341_BLACK);
  tft.setCursor(20, 160); //power factor
  tft.fillRect(20, 160, 300, 20, ILI9341_BLACK);
  tft.setCursor(20, 140); //ac voltage
  tft.fillRect(20, 140, 320, 20, ILI9341_BLACK);
  }
  Serial.println(mode);
  tft.setTextSize(5);
  tft.setCursor(30, 50);
  tft.setTextColor(ILI9341_WHITE);
  tft.println("V2V");
  tft.setTextSize(5);
  tft.setCursor(190, 50);
  tft.setTextColor(ILI9341_WHITE);
  tft.println("V2G");
  tft.setTextSize(2);
  tft.setCursor(20, 140);
  tft.setTextColor(ILI9341_WHITE);
  tft.println("BATTERY VOLTAGE :");
  tft.setCursor(240, 140);
  tft.fillRect(240, 140, 100, 20, ILI9341_BLACK);
  tft.println(dcvoltage);
  tft.setCursor(300, 140);
  tft.println("V");
  tft.setTextSize(2);
  tft.setCursor(20, 160);
  tft.fillRect(20, 160, 300, 20, ILI9341_BLACK);
  tft.setTextColor(ILI9341_WHITE);
  tft.println("SYSTEM TEMP :");
  tft.setCursor(200, 160);
  tft.setTextColor(ILI9341_WHITE);
  tft.println(temp);
  tft.setCursor(270, 160);
  tft.setTextColor(ILI9341_WHITE);
  tft.println("C");
  tft.setTextSize(2);
  tft.setCursor(20, 180);
  tft.setTextColor(ILI9341_WHITE);
  tft.println("SYSTEM STATUS : OFF");
  tft.setTextSize(2);
  tft.setCursor(20, 200);
  tft.setTextColor(ILI9341_WHITE);
  tft.println("TIME ELAPSED : 00:00:00");
  tft.setTextSize(2);
  tft.setCursor(20, 220);
  tft.setTextColor(ILI9341_WHITE);
  tft.println("ENERGY TRANSFERRED : 0W");
  clear=false;
  defaultupdate();
  analogWrite(A0,HIGH);
  delay(50);
}
}

void v2vtext()
{
  current_time = millis();
  stats="V2V";
  mode=1;
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
//  drawSwitchv2v(30, 30, v2v);
 // drawSwitchv2g(190, 30, v2g);
  Serial.println(mode);
  tft.setTextSize(5);
  tft.setCursor(30, 50);
  tft.setTextColor(ILI9341_WHITE);
  tft.println("V2V");
  tft.setTextSize(5);
  tft.setCursor(190, 50);
  tft.setTextColor(ILI9341_WHITE);
  tft.println("V2G");
  tft.setTextSize(2);
  tft.setCursor(20, 140);
  tft.setTextColor(ILI9341_WHITE);
  tft.println("BATTERY VOLTAGE :");
  tft.setCursor(240, 140);
  tft.fillRect(240, 140, 50, 20, ILI9341_BLACK);
  tft.println(dcvoltage);
  tft.setCursor(300, 140);
  tft.println("V");
  tft.setTextSize(2);
  tft.setCursor(20, 160);
  tft.fillRect(20, 160, 300, 20, ILI9341_BLACK);
  tft.setTextColor(ILI9341_WHITE);
  tft.println("SYSTEM TEMP :");
  tft.setCursor(200, 160);
  tft.setTextColor(ILI9341_WHITE);
  tft.println(temp);
  tft.setCursor(270, 160);
  tft.setTextColor(ILI9341_WHITE);
  tft.println("C");
  tft.setTextSize(2);
  tft.setCursor(20, 180);
  tft.setTextColor(ILI9341_WHITE);
  tft.println("SYSTEM STATUS :");
  tft.setCursor(200, 180);
  tft.fillRect(200, 180, 100, 20, ILI9341_BLACK);
  tft.println("V2V");
  tft.setTextSize(2);
  tft.setCursor(20, 200);
  tft.setTextColor(ILI9341_WHITE);
  tft.println("TIME ELAPSED :");
  tft.setTextSize(2);
  tft.setCursor(20, 220);
  tft.setTextColor(ILI9341_WHITE);
  tft.println("ENERGY TRANSFERRED:");
  tft.setCursor(245, 220);
  tft.fillRect(245, 220, 80, 20, ILI9341_BLACK);
  tft.println(dcenergy);
  tft.setCursor(300, 220);
  tft.println("W");
  analogWrite(A0,LOW);
  clear=true;
  v2vupdate();
  if(currentMillis1-previousMillis1 >= intervalupdate)
  {
  previousMillis1 = currentMillis1;
  voltage->save(acvoltage);
  battery->save(dcvoltage);
  status->save(stats);
  temperature->save(temp);
  Energy->save(dcenergy);
  //delay(5000);
  }
}
}
void v2gtext()
{
  current_time = millis();
  stats="V2G";
  mode=2;
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
 // drawSwitchv2v(30, 30, v2v);
 // drawSwitchv2g(190, 30, v2g);
  Serial.println(mode);
  tft.setTextSize(5);
  tft.setCursor(30, 50);
  tft.setTextColor(ILI9341_WHITE);
  tft.println("V2V");
  tft.setTextSize(5);
  tft.setCursor(190, 50);
  tft.setTextColor(ILI9341_WHITE);
  tft.println("V2G");
  tft.setTextSize(2);
  tft.setCursor(20, 140);
  tft.fillRect(20, 140, 320, 20, ILI9341_BLACK);
  tft.setTextColor(ILI9341_WHITE);
  tft.println("AC VOLTAGE :");
  tft.setCursor(200, 140);
  tft.println(acvoltage);
  tft.setCursor(250, 140);
  tft.println("V");
  tft.setTextSize(2);
  tft.setCursor(20, 160);
  tft.fillRect(20, 160, 300, 20, ILI9341_BLACK);
  tft.setTextColor(ILI9341_WHITE);
  tft.println("POWER FACTOR :");
  tft.setCursor(200, 160);
  tft.println(pf);
  tft.setTextSize(2);
  tft.setCursor(20, 180);
  tft.setTextColor(ILI9341_WHITE);
  tft.println("SYSTEM STATUS :");
  tft.setCursor(200, 180);
  tft.fillRect(200, 180, 100, 20, ILI9341_BLACK);
  tft.println("V2G");
  tft.setTextSize(2);
  tft.setCursor(20, 200);
  tft.setTextColor(ILI9341_WHITE);
  tft.println("TIME ELAPSED :");
  tft.setTextSize(2);
 tft.setCursor(20, 220);
  //tft.fillRect(20, 220, 310, 20, ILI9341_BLACK);
  tft.setTextColor(ILI9341_WHITE);
  tft.println("ENERGY TRANSFERRED:");
  tft.setCursor(245, 220);
  tft.fillRect(245, 220, 80, 20, ILI9341_BLACK);
  tft.println(acenergy);
  tft.setCursor(300, 220);
  tft.println("W");
  analogWrite(A0,LOW);
  clear=true;
  v2gupdate();
  //delay(5000);
}
}

void defaultupdate()
{ 
  if(currentMillis1-previousMillis1 >= intervalupdate)
  {
  previousMillis1 = currentMillis1;
  voltage->save(acvoltage);
  battery->save(dcvoltage);
  status->save(stats);
  temperature->save(temp);
  Energy->save(energy);
  Serial.println("offffff");
}
}

void v2vupdate()
{
  if(currentMillis1-previousMillis1 >= intervalupdate)
  {
  previousMillis1 = currentMillis1;
  voltage->save(acvoltage);
  battery->save(dcvoltage);
  status->save(stats);
  temperature->save(temp);
  Energy->save(dcenergy);
  Serial.println("v2222vvvv");
}
}

void v2gupdate()
{
  if(currentMillis1-previousMillis1 >= intervalupdate)
  {
  previousMillis1 = currentMillis1;
  voltage->save(acvoltage);
  battery->save(dcvoltage);
  status->save(stats);
  temperature->save(temp);
  Energy->save(acenergy);
  Serial.println("v22222222gggggg");
}
}

void handleMessage(AdafruitIO_Data *data) {

  //Serial.print("received <- ");

  if(data->toPinLevel() == 1&&v2g==false)
  {
    //Serial.println("HIGH");
      v2v=true;
      drawSwitchv2v(30, 30, v2v);
      delay(50); // debounce delay
      v2vtext();
  }
  if(data->toPinLevel() == 0)
  {
   // Serial.println("LOW");
      v2v=false;
      delay(50); // debounce delay
      drawSwitchv2v(30, 30, false);
      drawSwitchv2g(190, 30, false);
      defaulttext();
  }

}

void handleMessage2(AdafruitIO_Data *data) {

  //Serial.print("received 2 <- ");

  if(data->toPinLevel() == 1 &&v2v==false)
  {
    //Serial.println("HIGH");
    v2g=true;
      drawSwitchv2g(190, 30, v2g);
      delay(50); // debounce delay
     v2gtext();
  }
  if(data->toPinLevel() == 0)
  {
    //Serial.println("LOW");
      v2v=false;
      drawSwitchv2v(30, 30, false);
      drawSwitchv2g(190, 30, false);
      delay(50); // debounce delay
      defaulttext();
  }

}