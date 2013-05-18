#include <SAT_Clock.h>
#include <Wire.h>
#include <I2C_add.h>
#include <nanosat_message.h>
#include <OnboardCommLayer.h>
#include <System.h>
#include <SAT_AppStorage.h>
#include <SAT_ExperimentData.h>


uint8_t nodeid = 1;
System mysystem = System(nodeid);
SAT_AppStorage storage = SAT_AppStorage(nodeid);
SAT_Clock myclock= SAT_Clock();

void setup(){
  Serial.begin(115200);
  myclock.init(nodeid);
}

void loop(){
  
  float myTime = myclock.getTime();
 
  storage.store_exp_data((uint8_t*)&myTime, sizeof(float));
  
  delay(1000);
  
  mysystem.exit();
}
