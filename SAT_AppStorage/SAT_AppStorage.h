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

#ifndef SAT_APPSTORAGE_H
#define SAT_APPSTORAGE_H

#include <inttypes.h>
#include <stdlib.h>

#include <nanosat_message.h>
#include <OnboardCommLayer.h>
#include <SAT_ExperimentData.h>


class SAT_AppStorage
{
    private:
        uint8_t _local_address;         //local address of this node
        OnboardCommLayer* commLayer;
    public:
		/**Constructor that has a single ID parameter.
			@param id - The id of the user's arduino on the ArduSat. This allows 
			the supervisor to know which arduino node to send the data back to. 
			The id is assigned by NanoSatisfi.
        */
        SAT_AppStorage(uint8_t id);
		/**The first way to store data. 
			@param edata - A pointer to experiment data. See: SAT_ExperimentData.h
        */
        void store_exp_data(SAT_ExperimentData* edata);
		/**A alternate way to store data using a buffer. 
			@param buf - A pointer to a data buffer.
			@param len - The length in bytes of the buffer.
        */
        void store_exp_data(const uint8_t* buf, size_t len);
};

#endif /** This corresponds with the dstore header card */
