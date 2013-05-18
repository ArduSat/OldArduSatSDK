/**
   @brief SAT_Control.cpp - Library to write arduino application data from arduino->supervisor.

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
#include <nanosat_message.h>
#include "SAT_Control.h"

/******************************************************************************
 * Definitions
 ******************************************************************************/


/******************************************************************************
 * Constructors
 ******************************************************************************/
SAT_Control::SAT_Control(uint8_t id){
    _local_address = id;
    _commLayer = new OnboardCommLayer(MULTIMASTER, _local_address);
}

/******************************************************************************
 * Private functions 
 ******************************************************************************/


/******************************************************************************
 * User API
 ******************************************************************************/
void SAT_Control::changePitch(float degrees){
    memset(_buff,0,NODE_COMM_MAX_BUFFER_SIZE);
    nanosat_message_t* msg= (nanosat_message_t*)&_buff[0];
    msg->prefix = NODE_COMM_MESSAGE_PREFIX;
    msg->type = CONTROL_PITCH;
    msg->node_addr = _local_address;
    msg->len=0;
    float* payload = (float*)&msg->data;
    payload[0]=degrees;
    _commLayer->send(_buff, sizeof(nanosat_message_t), ADD_SUPERVISOR_CONTROL);
}

void SAT_Control::changeYaw(float degrees){
    memset(_buff,0,NODE_COMM_MAX_BUFFER_SIZE);
    nanosat_message_t* msg= (nanosat_message_t*)&_buff[0];
    msg->prefix = NODE_COMM_MESSAGE_PREFIX;
    msg->type = CONTROL_YAW;
    msg->node_addr = _local_address;
    msg->len=0;
    float* payload = (float*)&msg->data;
    payload[0]=degrees;
    _commLayer->send(_buff, sizeof(nanosat_message_t), ADD_SUPERVISOR_CONTROL);
}

void SAT_Control::changeRoll(float degrees){
    memset(_buff,0,NODE_COMM_MAX_BUFFER_SIZE);
    nanosat_message_t* msg= (nanosat_message_t*)&_buff[0];
    msg->prefix = NODE_COMM_MESSAGE_PREFIX;
    msg->type = CONTROL_ROLL;
    msg->node_addr = _local_address;
    msg->len=0;
    float* payload = (float*)&msg->data;
    payload[0]=degrees;
    _commLayer->send(_buff, sizeof(nanosat_message_t), ADD_SUPERVISOR_CONTROL);
}

