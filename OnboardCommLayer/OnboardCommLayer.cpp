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

static uint8_t transmitBuffer[sizeof(nanosat_message_t)];

OnboardCommLayer::OnboardCommLayer() {
  assvAddress_ = 0x04;
  nodeAddress_ = EEPROM.read(0x00);
}

uint8_t OnboardCommLayer::sendMessage(nanosat_message_t msg) {
  memcpy(transmitBuffer, &msg, sizeof(nanosat_message_t));
  Wire.begin(assvAddress_);
  Wire.beginTransmission(assvAddress_);
  Wire.write(transmitBuffer, sizeof(nanosat_message_t));
  Wire.endTransmission();
}

uint8_t OnboardCommLayer::sendExit() {
  nanosat_message_t msg = {
    NODE_COMM_MESSAGE_PREFIX,
    EXIT,
    EEPROM.read(0x00)
  };
  sendMessage(msg);
}
