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
#include <OnboardCommLayer.h>	// for OBCL

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

void SAT_Temp::init() { // // jfomhover on 07/08/2013 : uint8_t nodeid not needed
//  _local_address = nodeid;
	OBCL.begin();
}

float SAT_Temp::getTemp(){
  int TemperatureSum = getRawTemp();

  float celsius = TemperatureSum*0.0625;
  return celsius;
}

int16_t SAT_Temp::getRawTemp() {
	int16_t TemperatureSum = 0;
	int8_t t_ret = OBCL.request16bits(_temp_i2c_addr, (uint16_t*)(&TemperatureSum), true, I2C_COMM_INSTANTTIMEOUT);
	// TODO : what should we do here if there's an error ?

	//it's a 12bit int, using two's compliment for negative
  TemperatureSum = TemperatureSum >> 4;
  // TODO : use two's compliment for negative somehow ?
  return(TemperatureSum);
}
