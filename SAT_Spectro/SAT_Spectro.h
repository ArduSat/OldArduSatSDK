/** @brief  Library to read the spectrometer.
	
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


#ifndef SPECTRO_H
#define SPECTRO_H

#include <inttypes.h>
#include <stdlib.h>

#include <Arduino.h>
#include <nanosat_message.h>
#include <OnboardCommLayer.h>

#define SEPCTRO_SAMPLE_SIZE 500

class SAT_Spectro
{
    private:
        uint8_t _local_address;         //local address of this node
        OnboardCommLayer* commLayer;
        char m[3];

    public:
		/**Constructor that has a single ID parameter.
			@param id - The id of the user's arduino on the ArduSat. This allows 
			the supervisor to know which arduino node to send the data back to. 
			The id is assigned by NanoSatisfi.
        */
        SAT_Spectro(uint8_t id);
		
		/**Sets the exposure on the spectrometer.
			@param setting - A value from 1-99.
        */
        void set_exposure(uint8_t setting);
		
		/**Initiates the taking from a spectrometer sample */
        void take_sample(void);

        /**
         * @param function(size_t, boolean) size_t is returned chunk size,
         *                                  the boolean is a done flag
         * @param appbuf a pointer to the application buffer to be
         *          filled with a chunk of data from the spectruino
         * @param buffer_len the maximum buffer size of the application buffer
         */
        void onReceiveChunk(void (*) (size_t, boolean), byte*, size_t);
};

#endif
