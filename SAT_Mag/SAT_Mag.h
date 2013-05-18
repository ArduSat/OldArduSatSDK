/** @brief  Library for Arduino returns x,y,z data on current magnetic field

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
	
	Designed for use with Freescale (C) MAG3110 Magnetometer
   
	Tested with MAG3110 Breakout from Sparkfun and Arduino Uno
	
	Based on Sparkfun's example for the MAG3110 breakout board:
	http://dlnmh9ip6v2uc.cloudfront.net/datasheets/BreakoutBoards/Mag3110_v10.pde
	
    @author Jeroen Cappaert & Lara Booth for NanoSatisfi
    @author Jorge Ortiz
    @date May 2013
*/

#ifndef SAT_Mag_h
#define SAT_Mag_h

#include <Arduino.h>
#include <OnboardCommLayer.h>


class SAT_Mag
{
  public:
	/**Constructor that has a single ID parameter. */
    SAT_Mag();

   	/**Constructor that has a single ID parameter.
			@param node_id - The id of the user's arduino on the ArduSat. This allows 
			the supervisor to know which arduino node to send the data back to. 
			The id is assigned by NanoSatisfi.
    */
    void init(uint8_t node_id);
	
	/**Initializes the magnetometer. */
    void configMag(); 
	
	/**Reads the x vector. */
    int readx();
    
	/**Reads the y vector. */
	int ready();
	
	/**Reads the z vector. */
    int readz();
	
	/**Reads the x offset value. */
    float x_value();
	
	/**Reads the y offset value. */
    float y_value();
    
	/**Reads the z offset value. */
	float z_value();
	
	/**Returns the heading. 
		@param x - the x vector. Typically, you would pass the value you recieved from readx();
		@param y - the y vector. Typically, you would pass the value you recieved from ready();
		@param z - the z vector. Typically, you would pass the value you recieved from readz();
	*/
    int getHeading(float x, float y, float z);

  private:
    uint8_t _local_address;
    OnboardCommLayer* _ocl;
    byte _buff[2];

    //class-only methods:
    int read16Data(char r);
    
    //global variables
    float mag_x_scale;
    float mag_y_scale;
    float mag_z_scale;
    
};


#endif
