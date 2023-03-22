#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX

void setup() {
  Serial.begin(9600); // Set baud rate to match serial monitor
  mySerial.begin(9600); // Set baud rate to match transmitter
}

void loop() {
  if (mySerial.available() > 0) { // Check if there is incoming data
    String receivedData = mySerial.readStringUntil('D'); // Read the incoming data until the 'D' delimiter
    Serial.print("Received data: ");
    Serial.println(receivedData); // Print the received data
    
    // Extract the different values from the received data
    int currIndex = receivedData.indexOf('C'); // Find the index of the 'C' character
    float current = receivedData.substring(currIndex + 1, currIndex + 6).toFloat(); // Extract the current value
    
    int voltageIndex = receivedData.indexOf('V'); // Find the index of the 'V' character
    float voltage = receivedData.substring(voltageIndex + 1, voltageIndex + 6).toFloat(); // Extract the voltage value
    
    int whIndex = receivedData.indexOf("Wh"); // Find the index of the "Wh" characters
    float energy = receivedData.substring(whIndex - 5, whIndex).toFloat(); // Extract the energy value
    
    int ahIndex = receivedData.indexOf("Ah"); // Find the index of the "Ah" characters
    float charge = receivedData.substring(ahIndex - 4, ahIndex).toFloat(); // Extract the charge value
    
    // Print the extracted values
    Serial.print("Current: ");
    Serial.print(current);
    Serial.print(" A, Voltage: ");
    Serial.print(voltage);
    Serial.print(" V, Energy: ");
    Serial.print(energy);
    Serial.print(" Wh, Charge: ");
    Serial.print(charge);
    Serial.println(" Ah");
  }
}