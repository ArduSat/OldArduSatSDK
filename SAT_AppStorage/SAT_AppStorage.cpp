/*
 *  @file SAT_AppStorage.cpp
*   @version 0.1
*   @name NanoSatisfi Inc.
*
*   @section LICENSE
*
*   see LICENSE and NOTICE files.
*
*   @section DESCRIPTION
*
*   Library to write arduino application data from arduino->satellite(assv).
 */

/******************************************************************************
 * Includes
 ******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <I2C_add.h>
#include "SAT_AppStorage.h"
#include "nanosat_message.h"
#include "EEPROM.h"

/******************************************************************************
 * Constructors
 ******************************************************************************/
SAT_AppStorage::SAT_AppStorage()
{
  commLayer_    = OBCL;
  nodeAddress_  = EEPROM.read(0x00);
}

/******************************************************************************
 * User API
 ******************************************************************************/
/*
   Passes the experiment data to the comm layer; the comm layer enqueues if for
   sending upon request from master;

   @note There is a 100ms delay to allow time for FS work.
*/
void SAT_AppStorage::send(char data[])
{
  unsigned int dataLen  = (unsigned)strlen(data);
  unsigned int messages = dataLen / NODE_COMM_MAX_BUFFER_SIZE;
  for(unsigned int i = 0; i < messages; i++)
  {
    unsigned int start_offset   = i * NODE_COMM_MAX_BUFFER_SIZE;
    copyAndSend((byte*) data, start_offset, NODE_COMM_MAX_BUFFER_SIZE);
  }
  // process remainder or if data was less then NODE_COMM_MAX_BUFFER_SIZE;
  uint8_t remainderLen = dataLen % NODE_COMM_MAX_BUFFER_SIZE;
  uint8_t finalOffset  = (dataLen > NODE_COMM_MAX_BUFFER_SIZE) ?
    (messages * NODE_COMM_MAX_BUFFER_SIZE) : 0;
  copyAndSend((byte*) data, finalOffset, remainderLen);
}

void SAT_AppStorage::send(byte *data, unsigned int start, unsigned int length)
{
  unsigned int dataLen  = (unsigned)(length);
  unsigned int messages = dataLen / NODE_COMM_MAX_BUFFER_SIZE;

  for(unsigned int i = 0; i < messages; i++)
  {
    unsigned int start_offset   = i * NODE_COMM_MAX_BUFFER_SIZE;
    copyAndSend((byte*) data, start+start_offset, NODE_COMM_MAX_BUFFER_SIZE);
  }
  // process remainder or if data was less then NODE_COMM_MAX_BUFFER_SIZE;
  uint8_t remainderLen = dataLen % NODE_COMM_MAX_BUFFER_SIZE;
  uint8_t finalOffset  = (dataLen > NODE_COMM_MAX_BUFFER_SIZE) ?
    (messages * NODE_COMM_MAX_BUFFER_SIZE) : 0;
  copyAndSend((byte*) data, start+finalOffset, remainderLen);
}

void SAT_AppStorage::copyAndSend(
  byte data[], unsigned int offset, unsigned int length)
{
  nanosat_message_t msg;
  msg.node_addr = nodeAddress_;
  msg.prefix    = NODE_COMM_MESSAGE_PREFIX;
  msg.len       = length;
  msg.type      = APPEND;
  memcpy(msg.buf, (uint8_t*)&(data[offset]), length * sizeof(char));
  commLayer_.sendMessage(msg);
  delay(100); // jfomhover on 08/12/2013 : wouldn't it be better to flush the wire on commLayer_ instead of introducing delay ?
}
