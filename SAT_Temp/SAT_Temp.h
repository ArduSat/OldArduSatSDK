/** @brief  Library to query the on board temperature sensors.
	
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


#ifndef TEMP_H 
#define TEMP_H

#include <inttypes.h>

#include <nanosat_message.h>
#include <OnboardCommLayer.h>

class SAT_Temp{
    private:
        uint8_t _local_address;
        uint8_t _temp_i2c_addr;
        byte _buff[2];
        OnboardCommLayer* _ocl;

    public:
		
		/**Tempurature sensor constructor
			@param tempid - The id of the tempurature sensor. There are four of these sensors.
        */
        SAT_Temp(uint8_t tempid);
		
		/**Initializes the tempurature sensor for this node.
			@param nodeid - The id of the user's arduino on the ArduSat. This allows 
			the supervisor to know which arduino node to send the data back to. 
			The id is assigned by NanoSatisfi.
        */
        void init(uint8_t nodeid);
		
		/**Returns the tempurature in Celsius from the sensor. */
        float getTemp();    //returns temperature in celcius
};

#endif
