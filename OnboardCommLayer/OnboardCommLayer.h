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


#ifndef ONBOARD_COMM_LAYER_H
#define ONBOARD_COMM_LAYER_H

#include <Arduino.h>
#include <I2C_add.h>
#include <nanosat_message.h>

class OnboardCommLayer{
  private:
    uint8_t assvAddress_;
    uint8_t nodeAddress_;
  public:
    OnboardCommLayer();
    uint8_t sendMessage(nanosat_message_t msg);
    uint8_t sendExit();
};

#endif /* ONBOARD_COMM_LAYER_H */
