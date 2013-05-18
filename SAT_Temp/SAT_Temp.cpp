/**
   @brief Temperature Sensor Library

   Copyright 2013 NanoSatisfi, Inc.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
   
   @author Jorge Ortiz and NanoSatisfi, Inc.
   @date May 2013
  
*/

/******************************************************************************
 * Includes
 ******************************************************************************/
#include <I2C_add.h>
#include "SAT_Temp.h"

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
    _ocl=new OnboardCommLayer(MULTIMASTER, _local_address);
}

float SAT_Temp::getTemp(){
    _ocl->requestDataFrom(_temp_i2c_addr, 2, _buff, 2);

    byte MSB = _buff[0];
    byte LSB = _buff[1];

    //it's a 12bit int, using two's compliment for negative
    int TemperatureSum = ((MSB << 8) | LSB) >> 4; 

    float celsius = TemperatureSum*0.0625;
    return celsius;
}
