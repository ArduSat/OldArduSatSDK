#include <System.h>
#include <SAT_AppStorage.h>
#include <SAT_ExperimentData.h>
#include <Wire.h>
#include <Serial.h>
#include <I2C_add.h>
#include <nanosat_message.h>
#include <OnboardCommLayer.h>
#include <SAT_Temp.h>

uint8_t nodeid = 1;
SAT_AppStorage storage = SAT_AppStorage(nodeid);
System mysys = System(nodeid);

SAT_Temp temp = SAT_Temp(3/*1-4 temperature sensors available*/);

void setup(){
  Serial.begin(115200);
  temp.init(1/*node*/);
}

void loop(){
  float t = temp.getTemp();
  Serial.println(t);
  delay(1000);
  storage.store_exp_data((uint8_t*)&t, sizeof(float));
  mysys.exit();
}
