/*
    SAT_Spectro.cpp - Library read spectrometer;
    Copyright (C) 2013 Jorge Ortiz for NanoSatisfi

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
    nanosat_message_t msg;
    memset(&msg, 0, sizeof(nanosat_message_t));
    msg.prefix = NODE_COMM_MESSAGE_PREFIX;
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
