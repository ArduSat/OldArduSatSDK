#include "SAT_Accel.h"

SAT_Accel * accel;
int x, y, z;

void setup() {
  Serial.begin(9600);
  accel = new SAT_Accel();
}

void loop() {
  accel->populateValues(x, y, z);
  Serial.print("X: "); Serial.println(x);
  Serial.print("Y: "); Serial.println(y);
  Serial.print("Z: "); Serial.println(z);
  delay(1000);
}
