#include <Wire.h>
#include <I2C_add.h>
#include <nanosat_message.h>
#include <OnboardCommLayer.h>
#include <System.h>

System system = System(1);

void setup(){
  Serial.begin(115200);
}

void loop(){
  system.exit();
  Serial.println("Exit");
  delay(1000);
}
