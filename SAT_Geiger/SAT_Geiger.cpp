 /** @brief  SAT_Geiger.cpp - Library to using Geiger Counter
 
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
	
    @author Jeroen Cappaert
    @date May 2013
*/

 #include <OnboardCommLayer.h>
 #include <Arduino.h>
 #include <I2C_Conv.h>
 #include <I2C_add.h>
 #include "SAT_Geiger.h"
 
 SAT_Geiger::SAT_Geiger(){
   
 }

 void SAT_Geiger::init(uint8_t node_id){
     _local_address=node_id;
     _ocl = new OnboardCommLayer(MULTIMASTER,node_id);
 }
 
long SAT_Geiger::getCPM(byte tube_handle){
    byte stat_reg=0;
    long cpm=0;

    stat_reg = CPM1;
    switch (tube_handle){
        case 2: 
            stat_reg = CPM2;
            break;
    }
    // set status reg to CPM1
    send(&stat_reg, 1);

    // wait for measurement
    delay(T_DELAY);

    // get data
    receive(buffer, BUF_SIZE);

    // convert bytes to value
    Byte2Long(cpm, buffer);

    // return
    return cpm;
}
 
float SAT_Geiger::getUSPH(byte tube_handle){
    byte stat_reg=0;
    float rad=0;

    stat_reg = USPH1;
    switch (tube_handle){
        case 2: 
            stat_reg = USPH2;
            break;
    }

    // set status reg to CPM1
    send(&stat_reg, 1);

    // wait for measurement
    delay(T_DELAY);

    // get data
    receive(buffer, BUF_SIZE);

    Byte2Float(rad, buffer);

    return rad;
}
 
 
 // simple I2C send wrapper 
 void SAT_Geiger::send(byte* data, unsigned int len){
   _ocl->send((uint8_t*)data, len, I2C_ADD_GEIGER);
 }
 
 // simple I2C receive wrapper
 void SAT_Geiger::receive(byte* data, unsigned int len){
   _ocl->requestDataFrom(I2C_ADD_GEIGER, len, data, len);
   return;
 }
