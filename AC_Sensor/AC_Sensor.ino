#include <PZEM004Tv30.h>
#include <Wire.h>

PZEM004Tv30 pzem(2, 3); // Software Serial pin 8 (RX) & 9 (TX)

void setup() {
  Serial.begin(9600);
  pzem.resetEnergy();

}

void loop() {
  float voltage = pzem.voltage();
  if (voltage != NAN) {
    Serial.print("Voltage: ");
    Serial.print(voltage);
    Serial.println("V");
  } else {
    Serial.println("Error reading voltage");
  }

  float current = pzem.current();
  if (current != NAN) {
    Serial.print("Current: ");
    Serial.print(current);
    Serial.println("A");
  } else {
    Serial.println("Error reading current");
  }

  float power = pzem.power();
  if (current != NAN) {
    Serial.print("Power: ");
    Serial.print(power);
    Serial.println("W");
  } else {
    Serial.println("Error reading power");
  }

  float energy = pzem.energy();
  if (current != NAN) {
    Serial.print("Energy: ");
    Serial.print(energy, 3);
    Serial.println("kWh");

  } else {
    Serial.println("Error reading energy");
  }

  float frequency = pzem.frequency();
  if (current != NAN) {
    Serial.print("Frequency:");
    Serial.print(frequency, 1);
    Serial.println("Hz");
  } else {
    Serial.println("Error reading frequency");
  }

  float pf = pzem.pf();
  if (current != NAN) {
    Serial.print("PF: ");
    Serial.println(pf);
  } else {
    Serial.println("Error reading power factor");
  }
String dataString = "V" + String(voltage, 1) + "D" + "C" + String(current, 2) + "D" + "P" + String(power, 2) + "D" + "E" + String(energy, 3) + "D" + "PF" + String(pf, 2);
  Serial.println(dataString);
  delay(2000);
}