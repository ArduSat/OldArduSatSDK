/*
*   @file nanosat_message.h
*   @version 0.1
*   @name NanoSatisfi Inc.
*
*   @section DESCRIPTION
*
*   Since each experiment node does not have its own SD card, so there is a
*   simple pub/sub queue for writing data.
*/

#ifndef SAT_APP_STORAGE_H
#define SAT_APP_STORAGE_H

#include <inttypes.h>
#include <stdlib.h>
#include <Arduino.h>

#include "nanosat_message.h"
#include "OnboardCommLayer.h"

class SAT_AppStorage
{
  private:
    OnboardCommLayer commLayer_;
    uint8_t nodeAddress_;

    /*
     * Takes a fixed size of data to be packed into a Nanosat message struct
     * then queued in I2C.
     *
     * @param data    A byte array of data.
     * @param offset  starting offset.
     * @param length  how many bytes to be copied.
     */
    void copyAndSend(byte data[], unsigned int offset, unsigned int length);
  public:
    /*
     * Constructor
     */
    SAT_AppStorage();

    /*
     * Simple way to enqueue data to be published to disk.
     *
     * @note        When formatting data, don't forget to add newline or
     *              carriage returns.
     * @param data  A formatted string that needs to be written to disk for
     *   retreival by ground station.
     */
    void send(char data[]);
    void send(byte *data, unsigned int start, unsigned int length);

};

#endif /* SAT_APP_STORAGE_H */
