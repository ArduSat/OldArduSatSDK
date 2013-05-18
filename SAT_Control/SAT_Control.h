/** @brief  This is the library to use for controlling the satellite.

    You can change the satellite's pitch, yaw, and roll. The ArduSat uses
	magnetorquers. It takes significant time for the satellite to rotate.
	Accuracy is +/- 10 degrees.
	
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


#ifndef CONTROL_H
#define CONTROL_H

#include <inttypes.h>

#include <nanosat_message.h>
#include <OnboardCommLayer.h>


class SAT_Control
{
    private:
        uint8_t _local_address;         //local address of this node
        OnboardCommLayer* _commLayer;
        byte _buff[NODE_COMM_MAX_BUFFER_SIZE];

    public:
		/**Constructor that has a single ID parameter.
			@param id - the id of the user's arduino on the ArduSat. This allows the 
			supervisor to know which arduino node to send the data back to. The id 
			is assigned by NanoSatisfi.
        */
        SAT_Control(uint8_t id);
		
		/**Constructor that has a single ID parameter.
			@param degrees - the number of degrees to modify the pitch.
        */
        void changePitch(float degrees);
		
		/**Constructor that has a single ID parameter.
			@param degrees - the number ofdegrees to modify the yaw.
        */
        void changeYaw(float degrees);
		
		/**Constructor that has a single ID parameter.
			@param degrees - the number of degrees to modify the roll.
        */
        void changeRoll(float degrees);
};

#endif
