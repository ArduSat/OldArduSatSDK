/* 
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

/******************************************************************************
 * Includes
 ******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <FIMU_ITG3200_ocl.h>

#include "SAT_Gyro.h"
/******************************************************************************
 * Definitions
 ******************************************************************************/


/******************************************************************************
 * Constructors
 ******************************************************************************/
SAT_Gyro::SAT_Gyro(){
}



/******************************************************************************
 * Private functions 
 ******************************************************************************/


/******************************************************************************
 * User API
 ******************************************************************************/

void SAT_Gyro::init(uint8_t id){
    _local_address = id;
    _commLayer = new OnboardCommLayer(MULTIMASTER, id);
    _gyroX=0.0;
    _gyroY=0.0;
    _gyroZ=0.0;

   gyro = ITG3200(); 

   //initialize the gyro
   gyro.init(I2C_ADD_GYR, _commLayer);
   delay(1000);
   gyro.zeroCalibrate(128,5);
}

void SAT_Gyro::set(){
   gyro.readGyro(&_gyroX, &_gyroY, &_gyroZ);
}

float SAT_Gyro::getPitch(){
    return _gyroY;
}

float SAT_Gyro::getYaw(){
    return _gyroZ;
}

float SAT_Gyro::getRoll(){
    return _gyroX;
}

