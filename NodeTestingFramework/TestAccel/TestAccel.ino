#include <System.h>
#include <SAT_AppStorage.h>
#include <SAT_ExperimentData.h>
#include <Wire.h>
#include <I2C_add.h>
#include <nanosat_message.h>
#include <OnboardCommLayer.h>
#include <SAT_Accel.h>


uint8_t nodeid = 1;
SAT_Accel accel;
SAT_AppStorage storage = SAT_AppStorage(nodeid);
System mysys = System(nodeid);

void setup(){
  Serial.begin(115200);
  accel.init(1);
}

void loop(){
  delay(1000);
  accel.set();
  float x = accel.getAccelX();
  float y = accel.getAccelY();
  float z = accel.getAccelZ();
  Serial.print("Accel_x=");Serial.println(x);
  storage.store_exp_data((uint8_t*)&x, sizeof(float));
  Serial.print("Accel_y=");Serial.println(y);
  storage.store_exp_data((uint8_t*)&y, sizeof(float));
  Serial.print("Accel_z=");Serial.println(z);
  storage.store_exp_data((uint8_t*)&z, sizeof(float));
  Serial.println();
  mysys.exit();
  
}
