/** 
   brief SAT_Accel.cpp - Library to access accelerometer

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
#include "SAT_Accel.h"
#include "I2C_add.h"

/******************************************************************************
 * Definitions
 ******************************************************************************/


/******************************************************************************
 * Constructors
 ******************************************************************************/
SAT_Accel::SAT_Accel(){}

/******************************************************************************
 * Private functions 
 ******************************************************************************/
void SAT_Accel::init(uint8_t id){
    Serial.println("accel.init()");
    _local_address = id;
    _commLayer = new OnboardCommLayer(MULTIMASTER, _local_address);
    acc.init(I2C_ADD_ACC,_commLayer);
    //acc.init(I2C_ADD_ACC);
    _xyz_buff[0]=0;
    _xyz_buff[1]=0;
    _xyz_buff[2]=0;
    Serial.println("done");
}

void SAT_Accel::set(){
    Serial.println("setting accel");
    acc.get_Gxyz(_xyz_buff);
}

/******************************************************************************
 * User API
 ******************************************************************************/
float SAT_Accel::getAccelX(){
    return _xyz_buff[0];
}

float SAT_Accel::getAccelY(){
    return _xyz_buff[1];
}

float SAT_Accel::getAccelZ(){
    return _xyz_buff[2];
}


