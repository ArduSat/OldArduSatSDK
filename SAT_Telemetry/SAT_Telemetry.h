/** @brief  Library to access telemetry values (current) from the satellite.
	
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

#ifndef TELEMETRY_H
#define TELEMETRY_H

#include <inttypes.h>

#include <nanosat_message.h>
#include <OnboardCommLayer.h>

#define SIZE 32

class SAT_Telemetry
{
    private:
        uint8_t _local_address;         //local address of this node
        OnboardCommLayer* _commLayer;

        uint32_t _5V_current;
        uint32_t _3_3V_current;
        byte _send_buffer[SIZE];
        byte _buff[8];
        
        static boolean _valFetchDone;
        static void _onTelemetryFetchDone(size_t bytesRxd, boolean xferDone);

    public:
        /**Constructor that has a single ID parameter.
			@param id - The id of the user's arduino on the ArduSat. This allows 
			the supervisor to know which arduino node to send the data back to. 
			The id is assigned by NanoSatisfi.
        */
        SAT_Telemetry(uint8_t id);
		
		/**Sets the current values */
        void set();   
		
		/**Gets the 3 Volt Current value */     
        uint32_t get3_3V_Current();
		
		/**Gets the 5 Volt Current value */
        uint32_t get5V_Current();
};

#endif
