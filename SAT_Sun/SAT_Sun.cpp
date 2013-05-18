/** 
   @brief SAT_Sun.cpp - Library to write arduino application data from arduino->supervisor.


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
#include "SAT_Sun.h"

/******************************************************************************
 * Definitions
 ******************************************************************************/
extern float SAT_Sun::_val=-1.0;
extern boolean SAT_Sun::_valFetchDone = true;


/******************************************************************************
 * Constructors
 ******************************************************************************/
/*
 * Sun constructor.
 * 
 * @param id the unique if (1-16) of this node
 *
 */
SAT_Sun::SAT_Sun(uint8_t id){

    _local_address = id;
    _commLayer = new OnboardCommLayer(MULTIMASTER, _local_address);

    //time fetch flag
    SAT_Sun::_valFetchDone = true;

    //register the internal response handler from the supervisor
    //triggered when the time value has been received upon request
    _commLayer->onReceive(_onSunValueFetchDone, (byte*)&_val, 
            (uint8_t)sizeof(float), ADD_SUPERVISOR_SUN);
}


/******************************************************************************
 * Private functions 
 ******************************************************************************/
void SAT_Sun::_onSunValueFetchDone(size_t valSize, boolean isDone){
    SAT_Sun::_valFetchDone = true;
}


/******************************************************************************
 * User API
 ******************************************************************************/
/**
 * Gets the current time on satellite in milliseconds since Jan 1,1970.
 */
float SAT_Sun::getValue(uint8_t sun_id){
    int cycleTime = 100; //ms

    //populate the nanosat_message_t and camera-msg payload
    memset(&msg, 0, sizeof(nanosat_message_t));
    msg.prefix = NODE_COMM_MESSAGE_PREFIX;
    msg.type = SUN;
    msg.node_addr = _local_address;
    uint8_t* data_id_buf = (uint8_t*)&msg.data;
    data_id_buf[0] = sun_id;

    //send it
    SAT_Sun::_valFetchDone = false;
    _commLayer->send(&msg, ADD_SUPERVISOR_SUN);

    //wait until you hear something back
    while(!SAT_Sun::_valFetchDone){
        //Serial.println("w");
        delay(cycleTime);
    }
    Serial.println("broke out!");
    Serial.println((float)_val);
    
    return _val;
}
