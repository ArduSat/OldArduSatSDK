/**
   @brief SAT_Clock.cpp - Library to write arduino application data from arduino->supervisor.
   
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
#include "SAT_Clock.h"

/******************************************************************************
 * Definitions
 ******************************************************************************/
boolean SAT_Clock::_timeFetchDone = true;
uint8_t SAT_Clock::_local_address;
OnboardCommLayer* SAT_Clock::_commLayer=NULL;
float SAT_Clock::_time=0.0;
byte SAT_Clock::_send_buffer[SIZE];

/******************************************************************************
 * Constructors
 ******************************************************************************/
/*
 * Clock constructor.
 * 
 * @param id the unique if (1-16) of this node
 *
 */
SAT_Clock::SAT_Clock(){}


/******************************************************************************
 * Private functions 
 ******************************************************************************/
void SAT_Clock::_onTimeFetchDone(size_t timeSize, boolean isDone){
    _timeFetchDone = true;
}


/******************************************************************************
 * User API
 ******************************************************************************/
void SAT_Clock::init(uint8_t id){
    _local_address = id;
    _commLayer = new OnboardCommLayer(MULTIMASTER, _local_address);

    //time fetch flag
    _timeFetchDone = true;

    //register the internal response handler from the supervisor
    //triggered when the time value has been received upon request
    _commLayer->onReceive(_onTimeFetchDone, (byte*)&_time, 
            (size_t)sizeof(float), ADD_SUPERVISOR_CLOCK);
}

/**
 * Gets the current time on satellite in milliseconds since Jan 1,1970.
 */
float SAT_Clock::getTime(){
    int waitCycles = 0;
    int cycleTime = 20; //ms

    //populate the message_t and camera-msg payload
    memset(&_send_buffer, 0, SIZE);
    nanosat_message_t* msg = (nanosat_message_t*)&_send_buffer[0];
    msg->prefix = NODE_COMM_MAX_BUFFER_SIZE;
    msg->type = CLOCK;
    msg->node_addr = _local_address;

    _timeFetchDone = false;
    //send it
    _commLayer->send(msg, ADD_SUPERVISOR_CLOCK);

    //wait until you hear something back
    while(!_timeFetchDone){
        delay(cycleTime);
        waitCycles+=1;
    }

    //adjust the time to be closer to the current time
    _time += ((waitCycles*cycleTime)-(cycleTime/2));
    return _time;
}

SAT_Clock Clock = SAT_Clock();
