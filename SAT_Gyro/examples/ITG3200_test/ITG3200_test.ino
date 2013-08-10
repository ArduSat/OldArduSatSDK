// ITG-3200_test
// Copyright 2010-2011 Filipe Vieira & various contributors.
// http://code.google.com/p/itg-3200driver
// Simple test of gyro sensors output using default settings.

// (hardly) modified by jfomhover on Aug 2013 for the purpose of the ArduSatSDK

#include <Arduino.h>
#include <I2C_add.h>
#include <Wire.h>
#include <I2CComm.h>
#include <SAT_Gyro.h>

SAT_Gyro gyro;
float  x,y,z;
int ix, iy, iz;

void setup(void) {
  Serial.begin(115200);
  I2CComm.begin();   // if experiencing gyro problems/crashes while reading XYZ values
                     // please read class constructor comments for further info.
  delay(1000);
  // Use ITG3200_ADDR_AD0_HIGH or ITG3200_ADDR_AD0_LOW as the ITG3200 address 
  // depending on how AD0 is connected on your breakout board, check its schematics for details
  // on ArduSat, this value is I2C_ADD_GYR (see I2C_add.h)
  gyro.init(ITG3200_ADDR_AD0_LOW); 
  
  Serial.print("zeroCalibrating...");
  gyro.zeroCalibrate(2500, 2);
  Serial.println("done.");
}

void loop(void) {
  if (gyro.isRawDataReady()) {

      // Reads uncalibrated raw values from the sensor 
    gyro.readGyroRaw(&ix,&iy,&iz); 
    Serial.print("X1:"); 
    Serial.print(ix); 
    Serial.print("  Y:"); 
    Serial.print(iy); 
    Serial.print("  Z:"); 
    Serial.println(iz); 
/*     
    // Reads calibrated raw values from the sensor 
    gyro.readGyroRawCal(&ix,&iy,&iz); 
    Serial.print("X2:"); 
    Serial.print(ix); 
    Serial.print("  Y:"); 
    Serial.print(iy); 
    Serial.print("  Z:"); 
    Serial.println(iz); 
     
    // Reads calibrated values in deg/sec    
    gyro.readGyro(&x,&y,&z); 
    Serial.print("X3:"); 
    Serial.print(x); 
    Serial.print("  Y:"); 
    Serial.print(y); 
    Serial.print("  Z:"); 
    Serial.println(z);
*/
  } else {
    Serial.print('!');
  } 
}
