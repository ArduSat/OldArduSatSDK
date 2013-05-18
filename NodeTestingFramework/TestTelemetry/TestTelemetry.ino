#include <System.h>
#include <SAT_AppStorage.h>
#include <SAT_ExperimentData.h>
#include <Wire.h>
#include <Serial.h>
#include <I2C_add.h>
#include <nanosat_message.h>
#include <OnboardCommLayer.h>
#include <SAT_Telemetry.h>
#include <stdint.h>

uint8_t nodeid = 1;
SAT_AppStorage storage = SAT_AppStorage(nodeid);
System mysys = System(nodeid);

SAT_Telemetry tel = SAT_Telemetry(1);

void setup(){
  Serial.begin(15200);
}

void loop(){
  tel.set();
  delay(500);
  float current_3v = (float) tel.get3_3V_Current();
  float current_5v = (float) tel.get5V_Current();
  
  storage.store_exp_data((uint8_t*)&current_3v, sizeof(float));
  storage.store_exp_data((uint8_t*)&current_5v, sizeof(float));
  
  mysys.exit();
}
