/* 
    AppStorage.h - Library that signals, to the supervisor, the end of
    an experimental instance.

   

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
  
*/

#ifndef DSTORE_H
#define DSTORE_H

#include <inttypes.h>
#include <stdlib.h>
#include <Arduino.h>
#include <OnboardCommLayer.h>

class System
{
    private:
        uint8_t _local_address;
        OnboardCommLayer* _ocl;
        byte _buff[NODE_COMM_MAX_I2C_BUFFER_SIZE];
    public:
        System(uint8_t nodeid);
        void exit();
};

#endif
