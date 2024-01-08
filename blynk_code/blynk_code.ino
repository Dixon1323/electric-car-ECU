#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#define BLYNK_TEMPLATE_ID "TMPL3N53WIbrU"
#define BLYNK_TEMPLATE_NAME "Car ECU"
#define BLYNK_AUTH_TOKEN "-vO6d-vthcYolHQs2kymhWETzBZgFE_p"
#define BLYNK_PRINT Serial
#define button1_vpin    V4
#define button2_vpin    V5

WidgetLED led(V6);

bool v2v=false,v2g=true,state1=false;
char ssid[] = "Chathamkottu";
char pass[] = "12345678";
BLYNK_CONNECTED()
 {
  Blynk.syncVirtual(button1_vpin);
  Blynk.syncVirtual(button2_vpin);
 }

 BLYNK_WRITE(button1_vpin)
  {
  v2v = param.asInt();
 if(v2v==true&&v2g==false)
 {
   Serial.println("v2v");
   Blynk.virtualWrite(button1_vpin, v2v);
 }
 else
 if(v2v==false)
 {
   Serial.println("v2v offf");
 }
 else
 {
   Blynk.virtualWrite(button1_vpin, state1);
 }
}
//--------------------------------------------------------------------------
BLYNK_WRITE(button2_vpin) {
  v2g = param.asInt();
    if(v2g==true&&v2v==false)
 {
   Serial.println("v2g");
   Blynk.virtualWrite(button2_vpin, v2g);
 }
 else
 if(v2g==false)
 {
   Serial.println("v2g offf");
 }
 else
 {
   Blynk.virtualWrite(button2_vpin, state1);
 }
}

void setup()
{
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop() 
{
  float bat=48.3,energy=0.1;
  int temp=28,ac=230;
  Blynk.virtualWrite(V0, bat);
  Blynk.virtualWrite(V1, temp);
  Blynk.virtualWrite(V2, ac);
  Blynk.virtualWrite(V3, energy);
  led.on();
 // Blynk.virtualWrite(button1_vpin, false);
  //Blynk.virtualWrite(button2_vpin, false);
  Blynk.run();

  