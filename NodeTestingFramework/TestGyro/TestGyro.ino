#include <SAT_AppStorage.h>
#include <SAT_ExperimentData.h>
#include <Wire.h>
#include <I2C_add.h>
#include <nanosat_message.h>
#include <OnboardCommLayer.h>
#include <SAT_Gyro.h>
#include <System.h>

SAT_Gyro gyro;
uint8_t nodeid = 1;
SAT_AppStorage storage = SAT_AppStorage(nodeid);
System mysystem = System(nodeid);

void setup(){
  Serial.begin(115200);
  gyro.init(1);
}

void loop(){
  delay(1000);
  gyro.set();
  float x = gyro.getRoll();
  float y = gyro.getPitch();
  float z = gyro.getYaw();
  Serial.print("Roll=");Serial.println(x);
  Serial.print("Pitch=");Serial.println(y);
  Serial.print("Yaw=");Serial.println(z);
  storage.store_exp_data((uint8_t*)&x, sizeof(float));
  storage.store_exp_data((uint8_t*)&y, sizeof(float));
  storage.store_exp_data((uint8_t*)&z, sizeof(float));
  Serial.println();
  
  mysystem.exit();
}
