#include "SAT_Accel.h"
#include <Wire.h>
#include <EEPROM.h>
#include <OnboardCommLayer.h>

SAT_Accel accel;
int x, y, z;
double g;

void setup() {
  Serial.begin(9600);
  accel.powerOn();
}

void loop() {
  accel.readAccel(&x, &y, &z);
  Serial.print("X: "); Serial.println(x);
  Serial.print("Y: "); Serial.println(y);
  Serial.print("Z: "); Serial.println(z);
  
  accel.get_Gxyz(&g);
  Serial.print("G: "); Serial.println(g);
  
  delay(1000);
}