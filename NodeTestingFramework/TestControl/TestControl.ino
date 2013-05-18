#include <Wire.h>
#include <I2C_add.h>
#include <nanosat_message.h>
#include <OnboardCommLayer.h>
#include <SAT_Control.h>

SAT_Control control = SAT_Control(1/*nodeid*/);

void setup(){
  Serial.begin(115200);
}

void loop(){
  control.changePitch(90.00);
  control.changeYaw(90.00);
  control.changeRoll(90.00);
  delay(100001);
  //Every 1.6 minutes, change orientation 180 degrees
  //This is a poor test for real life because it's
  //Changing direction faster than the satellite
  //will actually be able to align itself.
}
