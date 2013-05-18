/** @brief  Library to write Arduino application data from Arduino to the supervisor.

    This is the library to use for writing experiment data that you would 
	like downloaded through the ground station network and available to 
	download from the control center.
	
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

#ifndef SAT_CLOCK_H
#define SAT_CLOCK_H

#include <inttypes.h>

#include <nanosat_message.h>
#include <OnboardCommLayer.h>

#define SIZE 32

class SAT_Clock
{
    private:
        static uint8_t _local_address;         //local address of this node
        static OnboardCommLayer* _commLayer;
        static float _time;  //milliseconds since epoch (must be confirmed)
        static byte _send_buffer[];
        static boolean _timeFetchDone;
        static void _onTimeFetchDone(size_t, boolean);

    public:
		/**Clock constructor */
        SAT_Clock();
		
		/**Initializes the clock. */
        void init(uint8_t);
		
		/**Returns the time from the clock as a float. */
        float getTime();
};

extern SAT_Clock Clock;

#endif
