#include <SAT_AppStorage.h>
#include <SAT_ExperimentData.h>
#include <Wire.h>
#include <I2C_add.h>
#include <nanosat_message.h>
#include <OnboardCommLayer.h>
#include <I2C_Conv.h>
#include <SAT_Mag.h>
#include <System.h>

SAT_Mag mag = SAT_Mag();
uint8_t nodeid = 1;
SAT_AppStorage storage = SAT_AppStorage(nodeid);
System mysys = System(nodeid);

void setup(){
  Serial.begin(115200);
  mag.init(1/*nodeid*/);
}

void loop(){
  delay(1000);
  int x = mag.readx();
  int y = mag.ready();
  int z = mag.readz();
  float offx = mag.x_value();
  float offy = mag.y_value();
  float offz = mag.z_value();
  Serial.print("x=");Serial.println(mag.readx());
  Serial.print("y=");Serial.println(mag.ready());
  Serial.print("z=");Serial.println(mag.readz());
  Serial.print("x_f=");Serial.println(mag.x_value());
  Serial.print("y_f=");Serial.println(mag.y_value());
  Serial.print("z_f=");Serial.println(mag.z_value());
  storage.store_exp_data((uint8_t*)&x, sizeof(int));
  storage.store_exp_data((uint8_t*)&y, sizeof(int));
  storage.store_exp_data((uint8_t*)&z, sizeof(int));
  storage.store_exp_data((uint8_t*)&offx, sizeof(float));
  storage.store_exp_data((uint8_t*)&offy, sizeof(float));
  storage.store_exp_data((uint8_t*)&offz, sizeof(float));
  Serial.println();
  mysys.exit();
  
}
