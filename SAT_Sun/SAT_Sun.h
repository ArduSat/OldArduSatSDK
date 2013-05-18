/** @brief  Library to read sun sunsor values.

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
#ifndef SUN_H
#define SUN_H

#include <inttypes.h>

#include <nanosat_message.h>
#include <OnboardCommLayer.h>

class SAT_Sun
{
    private:
        uint8_t _local_address;         //local address of this node
        OnboardCommLayer* _commLayer;
        nanosat_message_t msg;
        static float _val;  
        static boolean _valFetchDone;
        static void _onSunValueFetchDone(size_t, boolean);

    public:
		/**Constructor that has a single ID parameter.
			@param id - The id of the user's arduino on the ArduSat. This allows 
			the supervisor to know which arduino node to send the data back to. 
			The id is assigned by NanoSatisfi.
		*/
        SAT_Sun(uint8_t id);
		
		/**Gets a value from a particular sun sensor.
			@param sun_id - The id of the sun sensor to query. The range is 1-6.
        */
        float getValue(uint8_t sun_id);
};

#endif
