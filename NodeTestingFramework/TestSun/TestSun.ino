#include <SAT_Sun.h>
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
SAT_Sun sunsensor1(nodeid);

void setup(){
  Serial.begin(115200);
}

void loop(){
  
  float v1= sunsensor1.getValue((uint8_t)1);
  float v2= sunsensor1.getValue((uint8_t)2);
  float v3= sunsensor1.getValue((uint8_t)3);
  float v4= sunsensor1.getValue((uint8_t)4);
  float v5= sunsensor1.getValue((uint8_t)5);
  float v6= sunsensor1.getValue((uint8_t)6);
 
  storage.store_exp_data((uint8_t*)&v1, sizeof(float));
  storage.store_exp_data((uint8_t*)&v2, sizeof(float));
  storage.store_exp_data((uint8_t*)&v3, sizeof(float));
  storage.store_exp_data((uint8_t*)&v4, sizeof(float));
  storage.store_exp_data((uint8_t*)&v5, sizeof(float));
  storage.store_exp_data((uint8_t*)&v6, sizeof(float));
  
  delay(1000);
  
  mysystem.exit();
}
