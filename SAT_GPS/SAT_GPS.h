/** @brief  Library to get location data from the ArduSat.
	
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


#ifndef GPS_H
#define GPS_H

#include <inttypes.h>

#include <nanosat_message.h>
#include <OnboardCommLayer.h>


class SAT_GPS
{
    private:
        uint8_t _local_address;         //local address of this node
        OnboardCommLayer* _commLayer;
        unsigned int _velocity;
        float _lat;
        float _long;
        float _alt;
        byte _buff[NODE_COMM_MAX_BUFFER_SIZE];

    public:
		/**Constructor that has a single ID parameter.
			@param id - The id of the user's arduino on the ArduSat. This allows 
			the supervisor to know which arduino node to send the data back to. 
			The id is assigned by NanoSatisfi.
        */
        SAT_GPS(uint8_t id);

        /**Fetches and sets the current latitude, longitude, altitude.  
          Should be called before taking the other readings.
         */
        void set();
		
		/**Returns the Lattitude of the satellite as a float. */
        float getLat();

		/**Returns the Longitude of the satellite as a float. */
        float getLong();
		
		/**Returns the Longitude of the satellite as a long. */
        long getAltitude();
		
		/**Returns the velocity of the satellite as an unsigned int. */
        unsigned int getVelocity();
};

#endif
