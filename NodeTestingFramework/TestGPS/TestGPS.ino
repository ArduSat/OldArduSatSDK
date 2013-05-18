#include <SAT_GPS.h>
#include <Wire.h>
#include <I2C_add.h>
#include <nanosat_message.h>
#include <OnboardCommLayer.h>
#include <System.h>
#include <SAT_AppStorage.h>
#include <SAT_ExperimentData.h>


SAT_GPS gps = SAT_GPS(1/*nodeid*/);
uint8_t nodeid = 1;
System mysystem = System(nodeid);
SAT_AppStorage storage = SAT_AppStorage(nodeid);

void setup(){
  Serial.begin(115200);
}

void loop(){
  gps.set();
  float lat = gps.getLat();
  float lng = gps.getLong();
  float alt = gps.getAltitude();
  unsigned int v = gps.getVelocity();
  storage.store_exp_data((uint8_t*)&lat, sizeof(float));
  storage.store_exp_data((uint8_t*)&lng, sizeof(float));
  storage.store_exp_data((uint8_t*)&alt, sizeof(float));
  storage.store_exp_data((uint8_t*)&v, sizeof(unsigned int));
  delay(1000);
  
  mysystem.exit();
}
