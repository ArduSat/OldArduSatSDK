/*
*   @file nanosat_message.h
*   @version 1.0
*   @name NanoSatisfi Inc.
*
*   @section LICENSE
*
*   see LICENSE and NOTICE files.
*
*   @section DESCRIPTION
*   Message passing schema between OnboardCommLayer (I2C wrapper) and Payload
*   supervisor. This interface exposes system calls to user space and sensor
*   access. The main interaction players are Payload Supervisor(assv) and
*   experiment nodes.
*/

/******************************************************************************
 * Includes
 ******************************************************************************/
#include "OnboardCommLayer.h"
#include "nanosat_message.h"
#include "I2C_add.h"
#include <EEPROM.h>
#include <Wire.h>
#include <I2CComm.h>

static uint8_t transmitBuffer[sizeof(nanosat_message_t)];

OnboardCommLayer::OnboardCommLayer() {
  assvAddress_ = I2C_ADD_ASSV_1;		// jfomhover on 07/08/2013 : should be the address of the master arduino in the Sat
  nodeAddress_ = EEPROM.read(0x00);		// reads the address of the node from EEPROM
}

uint8_t OnboardCommLayer::sendMessage(nanosat_message_t msg) {
  memcpy(transmitBuffer, &msg, sizeof(nanosat_message_t)); // jfomhover on 2013/08/12 : is this necessary since we're not multitasking ?
  I2CComm.transmitByteArray(assvAddress_,transmitBuffer,sizeof(nanosat_message_t),I2C_COMM_INSTANTTIMEOUT);
  I2CComm.flushWrite();
//  Wire.begin(nodeAddress_); 				// jfomhover on 07/08/2013 : join the I2C bus as a slave with its node address in the Sat
//  Wire.beginTransmission(assvAddress_);		// transmit to the master arduino in the Sat
//  Wire.write(transmitBuffer, sizeof(nanosat_message_t));
//  Wire.endTransmission();
}

uint8_t OnboardCommLayer::sendExit() {
  nanosat_message_t msg = {
    NODE_COMM_MESSAGE_PREFIX,
    EXIT,
    EEPROM.read(0x00)
  };
  sendMessage(msg);
}
