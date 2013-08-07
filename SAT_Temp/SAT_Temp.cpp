/*
    Copyright (C) 2013 Jorge Ortiz for NanoSatisfi

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

/******************************************************************************
 * Includes
 ******************************************************************************/
#include <I2C_add.h>
#include <Arduino.h>
#include "SAT_Temp.h"
#include <Wire.h>

/******************************************************************************
 * Definitions
 ******************************************************************************/

/******************************************************************************
 * Constructors
 ******************************************************************************/
SAT_Temp::SAT_Temp(uint8_t tempid){
  _temp_i2c_addr=I2C_ADD_TMP1;
  switch(tempid){
    case 2:
      _temp_i2c_addr = I2C_ADD_TMP2;
      break;
    case 3:
      _temp_i2c_addr = I2C_ADD_TMP3;
      break;
    case 4:
      _temp_i2c_addr = I2C_ADD_TMP4;
      break;
  }
}

/******************************************************************************
 * Private functions
 ******************************************************************************/

/******************************************************************************
 * User API
 ******************************************************************************/

void SAT_Temp::init(uint8_t nodeid){
  _local_address = nodeid;
}

float SAT_Temp::getTemp(){
  Wire.requestFrom(_temp_i2c_addr, (uint8_t)2);

  byte MSB = Wire.read();
  byte LSB = Wire.read();

  //it's a 12bit int, using two's compliment for negative
  int TemperatureSum = ((MSB << 8) | LSB) >> 4; 

  float celsius = TemperatureSum*0.0625;
  return celsius;
}

int16_t SAT_Temp::getRawTemp() {
  Wire.requestFrom(_temp_i2c_addr, (uint8_t)2);

  byte MSB = Wire.read();
  byte LSB = Wire.read();

  //it's a 12bit int, using two's compliment for negative
  int16_t TemperatureSum = ((MSB << 8) | LSB) >> 4; 
  return(TemperatureSum);
}