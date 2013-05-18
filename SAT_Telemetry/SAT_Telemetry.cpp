/** 
   @brief SAT_Telemetry.cpp - Library to write arduino application data from arduino->supervisor.

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

#include <stdio.h>
#include <string.h>
#include "SAT_Telemetry.h"

/******************************************************************************
 * Definitions
 ******************************************************************************/
extern boolean SAT_Telemetry::_valFetchDone = true;

/******************************************************************************
 * Constructors
 ******************************************************************************/
SAT_Telemetry::SAT_Telemetry(uint8_t id){
    _local_address = id;
    _commLayer = new OnboardCommLayer(MULTIMASTER, _local_address);

    _valFetchDone = true;

    _commLayer->onReceive(_onTelemetryFetchDone, _buff, sizeof(uint32_t)*2,
            ADD_SUPERVISOR_TELEMETRY);
    
}

/******************************************************************************
 * Private functions 
 ******************************************************************************/
void SAT_Telemetry::_onTelemetryFetchDone(size_t bytesRxd, boolean done){
    _valFetchDone = true;
}


/******************************************************************************
 * User API
 ******************************************************************************/
void SAT_Telemetry::set(){

    int cycleTime = 100;

    memset(_send_buffer, 0, SIZE);
    nanosat_message_t* msg = (nanosat_message_t*)&_send_buffer[0];
    msg->prefix=NODE_COMM_MESSAGE_PREFIX;
    msg->type = TELEMETRY;
    msg->node_addr = _local_address;
    msg->len = 0;

    _valFetchDone = false;
    _commLayer->send(msg, (uint8_t)ADD_SUPERVISOR_TELEMETRY);

    while(!SAT_Telemetry::_valFetchDone)
        delay(cycleTime);
    _5V_current = _buff[0];
    _3_3V_current = _buff[1];

}

uint32_t SAT_Telemetry::get3_3V_Current(){
    return _3_3V_current;
}

uint32_t SAT_Telemetry::get5V_Current(){
    return _5V_current;
}


