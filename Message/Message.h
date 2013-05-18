/**
	@brief  Message.h

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
    
	@author NanoSatisfi, Inc.
    @date May 2013
  
*/

#ifndef MESSAGE_H
#define MESSAGE_H

#include <inttypes.h>
#include <Arduino.h>

#define MAX_BUFFER_SIZE 16 
#define MESSAGE_PREFIX 0xAB

//non-i2c sensor identifiers
#define ADD_SUPERVISOR_SUN              0xC8    //200
#define ADD_SUPERVISOR_CAM              0xC9    //201
#define ADD_SUPERVISOR_CLOCK            0xCA    //202
#define ADD_SUPERVISOR_TELEMETRY        0xCB    //203

/******************************************************************************
 * message types
 ******************************************************************************/
enum message_type{
    APPEND,         //append to the experiment file
    READ,           //read a value from a sensor
    SET,            //set the value of sensor register
    EXIT,           //signal the end of an experiment instance
    SENSOR_REG,     //register the use of a sensor by a node
    CAM,            //camera message (to supervisor)
    CLOCK,          //clock message (to supervisor)
    SUN,            //sun message (to supervisor)
    TELEMETRY        //telemetry request
};

/******************************************************************************
 * individual types of messages
 ******************************************************************************/

// application data message
typedef struct{
    uint16_t len;
    uint8_t buf[MAX_BUFFER_SIZE];
} nano_datamsg_t;

// exit message
typedef struct{
    uint8_t exit_status;
} nano_exitmsg_t;

//msg to camera
typedef struct{
    uint8_t default_;
    uint8_t compressionRatio;
    uint32_t pictureSize;
    float radians;
    float latitude;
    float longitude;
} nano_cammsg_t;


/******************************************************************************
 * union of all message -- for compaction
 ******************************************************************************/
typedef union{
    nano_datamsg_t datamsg;
    nano_exitmsg_t exitmsg;
    nano_cammsg_t cammsg;
} data_t;

/******************************************************************************
 * message put on wire (to the supervisor)
 ******************************************************************************/
typedef struct {
    uint8_t prefix;
    uint8_t type;
    uint8_t node_addr; //source address
    uint8_t len;
    data_t data;
    uint16_t checksum;
} message_t;


//-----------------------------------------------------------
  // Encode a Fletcher checksum in the last 2 bytes of buffer
  //-----------------------------------------------------------
  
inline uint16_t fletcher_encode(byte buffer[], long count )
{
    int i;
    unsigned char c0 = 0;
    unsigned char c1 = 0;
     *( buffer + count - 1 ) = 0;
     *( buffer + count - 2 ) = 0;
    for( i = 0; i < count; i++)
    {
        c0 = c0 + *( buffer + i );
        c1 = c1 + c0;
    }
    /*
    *( buffer + count - 2 ) = c0 - c1;
    *( buffer + count - 1 ) = c1 - 2*c0;*/
    uint8_t hob = c0-c1;
    uint8_t lob = c1 - 2*c0;
    return (uint16_t)(hob<<8) | lob;
 }
 
 //-------------------------------------------------------------
 // Decode Fletcher Checksum. Returns zero if buffer error-free
 //-------------------------------------------------------------
 
inline long fletcher_decode(byte buffer[], long count )
{
  long result = 0;
  int i;
  unsigned char c0 = 0;
  unsigned char c1 = 0;
  for( i = 0; i < count; i++)
    {
      c0 = c0 + *( buffer + i );
      c1 = c1 + c0;
    }
  return((long)(c0 + c1)); // returns zero if buffer is error-free
}

#endif
