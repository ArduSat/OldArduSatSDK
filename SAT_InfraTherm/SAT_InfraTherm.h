/** @brief  Library for Arduino returns data on temperature from MLX90614 sensor

	Notes:
	Designed for use with Melexis (C) MLX90614 Infrared Thermometer
	Library gathers temperature data over I2C Bus/SM Bus
	Tested with MLX90614 from Sparkfun, 4.7k Ohm pull-up resistors, .1uF capacitor, and Arduino Uno
	
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
	
    @author Lara Booth for NanoSatisfi, Inc.
    @date May 2013
*/

#ifndef SAT_InfraTherm_h
#define SAT_InfraTherm_h

//#define NS_MLX90614 0x5A //I2C address

#include <Arduino.h>
#include <OnboardCommLayer.h>

class SAT_InfraTherm
{
  public:
  /**Constructor */
  SAT_InfraTherm(); 

  /**Initializes the thermometer.
			@param node_id - The id of the user's arduino on the ArduSat. This allows 
			the supervisor to know which arduino node to send the data back to. 
			The id is assigned by NanoSatisfi.
  */
  void init(uint8_t node_id);
    
  /**Returns the tempurature in Celsius.  */
  float getTemp();
  
   
  private:

  byte _buff[3];
  uint8_t _local_address;
  OnboardCommLayer* _ocl;
  
  //class-only methods
  void rawTemperature(unsigned char r);
  
  //global variable
  double factor;
  double tempData;
 
};

#endif
