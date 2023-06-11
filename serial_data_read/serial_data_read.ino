#include <SoftwareSerial.h>

SoftwareSerial mySerial(D3,D0); // RX, TX

void setup()
 {
  // Start serial communication
  pinMode(3,INPUT);
  Serial.begin(9600);
  mySerial.begin(9600);
}

void loop() 
{
  // Read data from the module
  if (mySerial.available() > 0) 
  {
    String data = mySerial.readStringUntil('\n');
    Serial.println(data);
    
    // Extract voltage, energy, and charge values from the data
    float current = getValue(data, 'C');
    float voltage = getValue(data, 'V');
    float energy = getValue(data, 'W');
    float charge = getValue(data, 'A');
    
    // Print values to serial monitor
    /*Serial.print("Current:");
    Serial.print(current);
    Serial.print("Voltage: ");
    Serial.print(voltage);
    Serial.print(" V, Energy: ");
    Serial.print(energy);
    Serial.print(" Wh, Charge: ");
    Serial.print(charge);
    Serial.println(" Ah");*/
  }
}

float getValue(String data, char code) 
{
  int index = data.indexOf(code) + 1; // Skip the code character
  int endIndex = data.indexOf('D', index); // Find the end delimiter
  String valueStr = data.substring(index, endIndex);
  float value = valueStr.toFloat();
  return value;
}
