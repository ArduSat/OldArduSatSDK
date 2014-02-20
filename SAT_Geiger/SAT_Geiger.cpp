/* SAT_Geiger.cpp
 *
 * sensor library for I2C access to ArduSat geiger counter
 *
 * @author jeroencappaert
 * modified by jfomhover on Aug. 2013 for ArduSatSDK
 * v0.1
 */

#include <Arduino.h>
#include <I2C_Conv.h>
#include <I2C_add.h>
#include <Wire.h>
#include <EEPROM.h>
#include "SAT_Geiger.h"
#include <OnboardCommLayer.h>	// for OBCL

SAT_Geiger::SAT_Geiger(){
  //_local_address = EEPROM.read(0x00); // jfomhover 08/09/2013 : that's not for the sensor to do
}

long SAT_Geiger::getCPM(byte tube_handle){
  byte stat_reg = 0;
  long cpm      = 0;

  stat_reg = CPM1;
  switch (tube_handle){
      case 2: 
          stat_reg = CPM2;
          break;
  }

  // set status reg to CPM1
  //send(&stat_reg, 1);
  OBCL.transmitByte(I2C_ADD_GEIGER, stat_reg);
  // TODO: what to do if I2Ccomm returns negative value ?

  // wait for measurement
  delay(T_DELAY);

  // get data
  //receive(buffer, BUF_SIZE);
  uint8_t recdLen = 0;
  OBCL.requestByteArray(I2C_ADD_GEIGER, buffer, BUF_SIZE, &recdLen);
  // TODO: what to do if I2Ccomm returns negative value ?

  // convert bytes to value
  Byte2Long(cpm, buffer);

  // return
  return cpm;
}

float SAT_Geiger::getUSPH(byte tube_handle){
  byte stat_reg=0;
  float rad=0;

  stat_reg = USPH1;
  switch (tube_handle){
    case 2:
      stat_reg = USPH2;
      break;
  }

  // set status reg to CPM1
  //  send(&stat_reg, 1);
  OBCL.transmitByte(I2C_ADD_GEIGER, stat_reg);
  // TODO: what to do if I2Ccomm returns negative value ?

  // wait for measurement
  delay(T_DELAY);

  // get data
  //receive(buffer, BUF_SIZE);
  uint8_t recdLen = 0;
  OBCL.requestByteArray(I2C_ADD_GEIGER, buffer, BUF_SIZE, &recdLen);
  // TODO: what to do if I2Ccomm returns negative value ?

  Byte2Float(rad, buffer);

  return rad;
}

// simple I2C send wrapper
// jfomhover 08/09/2013 : replaced by OBCL functions
/*void SAT_Geiger::send(byte* data, unsigned int len){
  Wire.beginTransmission(I2C_ADD_GEIGER);
  Wire.write(data, len);
  Wire.endTransmission();
}*/

// simple I2C receive wrapper
// jfomhover 08/09/2013 : replaced by OBCL functions
/*void SAT_Geiger::receive(byte* data, unsigned int len){
  Wire.requestFrom(I2C_ADD_GEIGER, len);
  if (Wire.available())
  {
    for(unsigned int i = 0; i < len; i++)
    {
      data[i] = Wire.read();
    }
  }
}*/
