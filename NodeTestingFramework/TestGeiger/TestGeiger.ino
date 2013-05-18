#include <SAT_AppStorage.h>
#include <SAT_ExperimentData.h>
#include <Wire.h>
#include <I2C_add.h>
#include <nanosat_message.h>
#include <System.h>
#include <OnboardCommLayer.h>
#include <I2C_Conv.h>
#include <SAT_Geiger.h>


uint8_t nodeid = 1;
SAT_Geiger nuke = SAT_Geiger();
System mysystem = System(nodeid);
SAT_AppStorage storage = SAT_AppStorage(nodeid);

void setup(){
  Serial.begin(115200);


  nuke.init(1/*nodeid*/);
}

void loop(){
  for (int i = 2; i < 10; i++) { 
    
     float t1 =nuke.getUSPH((byte)1);
     storage.store_exp_data((uint8_t*)&t1, sizeof(float));
     
     float t2 =nuke.getUSPH((byte)2);
     storage.store_exp_data((uint8_t*)&t2, sizeof(float));
     
     long t3 =nuke.getCPM((byte)1);
     storage.store_exp_data((uint8_t*)&t3, sizeof(long));
     
     long t4 =nuke.getCPM((byte)2);
     storage.store_exp_data((uint8_t*)&t4, sizeof(long));
     
     delay(1000);
    
  }
  mysystem.exit();
   
}
