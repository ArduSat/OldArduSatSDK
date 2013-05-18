/** 
   @brief Gyroscope library
	
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

#ifndef GYRO_H
#define GYRO_H

#include <inttypes.h>
#include <nanosat_message.h>
#include "FIMU_ITG3200_ocl.h"

class SAT_Gyro
{
    private:
        uint8_t _local_address;         //local address of this node
        OnboardCommLayer* _commLayer;
        float _gyroX;
        float _gyroY;
        float _gyroZ;

        ITG3200 gyro;

    public:
        SAT_Gyro();
        float getPitch();
        float getYaw();
        float getRoll();
        void set();
        void init(uint8_t id);

};

#endif
