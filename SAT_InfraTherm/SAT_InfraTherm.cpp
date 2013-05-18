/** @brief  Library for Infrared Thermometer
	
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
   
    @date May 2013
	
////////////////////////////////////////////////////////////////////////////////
Notes:
Designed for use with Melexis (C) MLX90614 Infrared Thermometer
Library gathers temperature data over I2C Bus/SM Bus
Tested with MLX90614 from Sparkfun, 4.7k Ohm pull-up resistors, .1uF capacitor, and Arduino Uno
////////////////////////////////////////////////////////////////////////////////
*/

#include <I2C_add.h>
#include "SAT_InfraTherm.h"

//constructor
SAT_InfraTherm::SAT_InfraTherm(){
   //sets common values
   factor = 0.02; // MLX90614 has a resolution of .02
   tempData = 0x0000; //zero out the data
   
   }

void SAT_InfraTherm::init(uint8_t node_id){
    _local_address = node_id;
    _ocl = new OnboardCommLayer(MULTIMASTER, _local_address);
}
   
//modifies tempData into temperature in Kelvin
void SAT_InfraTherm::rawTemperature(unsigned char r)
{
     int data_low, data_high, pec;
     
     _ocl->send((uint8_t*)&r, 1, I2C_ADD_MLX);//sends register address to read

     _ocl->requestDataFrom(I2C_ADD_MLX, 3, _buff, 3); //request three bytes from device

     data_low = _buff[0]; //read first byte
     data_high = _buff[1]; //read second byte
     pec = _buff[2]; //read checksum

     // This masks off the error bit of the high byte, then moves it left 8 bits and adds the low byte.
     // Taken from bildr forum on MLX90614
     tempData =(double)(((data_high & 0x007F) << 8) + data_low);
     tempData = (tempData * factor)-0.01; //multiply by resolution and account for error to convert to Kelvin
}

//returns temperature in degrees Celsius
float SAT_InfraTherm::getTemp()
{
      rawTemperature(0x07); //modify tempData
      return (float) tempData - 273.15; //convert from Kelvin to Celsius
}