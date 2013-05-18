/** 
   @brief SAT_GPS.cpp - Library to write arduino application data from arduino->supervisor.

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
#include "SAT_GPS.h"

/******************************************************************************
 * Definitions
 ******************************************************************************/


/******************************************************************************
 * Constructors
 ******************************************************************************/
SAT_GPS::SAT_GPS(uint8_t id){
    _local_address = id;
    _commLayer = new OnboardCommLayer(MULTIMASTER, _local_address);

    //init vals
    _lat = 0; _long=0; _alt=0; _velocity=0;
    
}

/******************************************************************************
 * Private functions 
 ******************************************************************************/


/******************************************************************************
 * User API
 ******************************************************************************/
void SAT_GPS::set(){
    memset(_buff, 0, NODE_COMM_MAX_BUFFER_SIZE);
    _commLayer->requestDataFrom(I2C_ADD_GPS, sizeof(nanosat_gpsmsg_t), 
            _buff, NODE_COMM_MAX_BUFFER_SIZE);
    nanosat_gpsmsg_t* reply = (nanosat_gpsmsg_t*)&_buff[0];
    _lat = reply->altitude;
    _long = reply->longitude;
    _alt = reply->altitude;
    _velocity = reply->velocity;
}

float SAT_GPS::getLat(){
    return _lat;
}

float SAT_GPS::getLong(){
    return _long;
}

long SAT_GPS::getAltitude(){
    return _alt;
}

unsigned int SAT_GPS::getVelocity(){
    return _velocity;
}
