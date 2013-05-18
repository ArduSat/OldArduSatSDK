/* 
   @brief System.cpp

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

/******************************************************************************
 * Includes
 ******************************************************************************/

#include <Wire.h>
#include <nanosat_message.h>
#include "System.h"


/******************************************************************************
 * Definitions
 ******************************************************************************/


/******************************************************************************
 * Constructors
 ******************************************************************************/
System::System(uint8_t id){
    _local_address = id;
    _ocl = new OnboardCommLayer(MULTIMASTER, _local_address);
}

/******************************************************************************
 * User API
 ******************************************************************************/

void System::exit()
{
    memset(_buff, 0, NODE_COMM_MAX_I2C_BUFFER_SIZE);
    nanosat_message_t* msg = (nanosat_message_t*)&_buff[0];
    msg->prefix = NODE_COMM_MESSAGE_PREFIX;
    msg->type = EXIT;
    msg->node_addr = _local_address;
    _ocl->send(msg);
}

