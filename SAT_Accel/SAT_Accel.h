/**
	@brief  Accelerometer library 

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

#ifndef ACCEL_H
#define ACCEL_H

#include <inttypes.h>

#include <nanosat_message.h>
//#include <OnboardCommLayer.h>
#include "FIMU_ADXL345_ocl.h"
//#include "FIMU_ADXL345.h"


class SAT_Accel
{
    private:
        uint8_t _local_address;         //local address of this node
        OnboardCommLayer* _commLayer;
        float _xyz_buff[3];

        ADXL345 acc;

    public:
        SAT_Accel();
        void init(uint8_t id);
        void set();
        float getAccelX();
        float getAccelY();
        float getAccelZ();
};

#endif
