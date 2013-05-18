/** 
   @brief SAT_Spectro.cpp - Library read spectrometer;

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
#include <I2C_add.h>
#include "SAT_Spectro.h"

/******************************************************************************
* Initialize Class Variables
******************************************************************************/

/******************************************************************************
 * Definitions
 ******************************************************************************/


/******************************************************************************
 * Constructors
 ******************************************************************************/
SAT_Spectro::SAT_Spectro(uint8_t id){
    _local_address = id;
    commLayer = new OnboardCommLayer(MULTIMASTER, _local_address);

    //register the sensor (power related)
    message_t msg;
    memset(&msg, 0, sizeof(message_t));
    msg.prefix = MESSAGE_PREFIX;
    msg.type = SENSOR_REG;
    msg.node_addr = _local_address;
    msg.len = 1;
    uint8_t* _data = (uint8_t*)&msg.data;
    _data[0] = I2C_ADD_SPECT;
    commLayer->send(&msg);

}


/******************************************************************************
 * Private functions 
 ******************************************************************************/


/******************************************************************************
 * User API
 ******************************************************************************/
/*
 * Set the exposure time.  The set integer values range from 1-99;
 */
void SAT_Spectro::set_exposure(uint8_t setting){
    memset(&m[0],0,3);
    sprintf(m, "%c%d", '#', setting%100);
    commLayer->send((uint8_t*)m, strlen(m), I2C_ADD_SPECT);
}

/*
 * Request a sample from the spectruino.
 */
void SAT_Spectro::take_sample(){
    commLayer->requestDataFrom(I2C_ADD_SPECT, SEPCTRO_SAMPLE_SIZE);
}

/*
 *  Register a function to handle the reception of incoming *raw*
 *  data from the spectruino through the Communication Layer.
 *  If necessary, the function will be called many times until the
 *  entire payload is delivered.
 */
void SAT_Spectro::onReceiveChunk(void (*function)(size_t, boolean), 
        byte* appbuf, size_t buffer_len){
    commLayer->onReceive(function, appbuf, buffer_len, I2C_ADD_SPECT);
}
