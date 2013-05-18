/*
* @file nanosat_message.h
* @author Jorge Ortiz
* @author Borden Liu
* @version 1.0
* @name NanoSatisfi Inc.
*
* @section LICENSE
*
*  Copyright 2013 NanoSatisfi, Inc.
*
*   Licensed under the Apache License, Version 2.0 (the "License");
*   you may not use this file except in compliance with the License.
*   You may obtain a copy of the License at
*
*       http://www.apache.org/licenses/LICENSE-2.0
*
*   Unless required by applicable law or agreed to in writing, software
*   distributed under the License is distributed on an "AS IS" BASIS,
*   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*   See the License for the specific language governing permissions and
*   limitations under the License.
*
* @section DESCRIPTION
* Message passing schema between OnboardCommLayer (I2C wrapper) and Payload
* supervisor. This interface exposes system calls to user space and sensor
* access. The main interaction players are Payload Supervisor(assv) and
* experiment nodes.
*
* @todo Put examples here.
* @code Example
* @endcode
*/
#ifndef NANOSAT_MESSAGE_H
#define NANOSAT_MESSAGE_H

#include <inttypes.h>

#define NODE_COMM_MAX_I2C_BUFFER_SIZE 32
#define NODE_COMM_MAX_BUFFER_SIZE 16
#define NODE_COMM_MESSAGE_PREFIX 0xAB

//non-i2c sensor identifiers
#define ADD_SUPERVISOR_SUN 0xC8 //200
#define ADD_SUPERVISOR_CAM 0xC9 //201
#define ADD_SUPERVISOR_CLOCK 0xCA //202
#define ADD_SUPERVISOR_TELEMETRY 0xCB //203
#define ADD_SUPERVISOR_CONTROL 0xCC //204

/******************************************************************************
* message types
******************************************************************************/
enum message_type{
    APPEND, //append to the experiment file
    READ, //read a value from a sensor
    SET, //set the value of sensor register
    EXIT, //signal the end of an experiment instance
    SENSOR_REG, //register the use of a sensor by a node
    CAM, //camera message (to supervisor)
    CLOCK, //clock message (to supervisor)
    SUN, //sun message (to supervisor)
    TELEMETRY, //telemetry request
    CONTROL_PITCH, //change pitch
    CONTROL_YAW, //change yaw
    CONTROL_ROLL //change roll
};

/******************************************************************************
* individual types of messages
******************************************************************************/

// application data message
typedef struct __attribute__((packed)) {
    uint16_t len;
    uint8_t buf[NODE_COMM_MAX_BUFFER_SIZE];
} nanosat_datamsg_t;

// exit message
typedef struct __attribute__((packed)) {
    uint8_t exit_status;
} nanosat_exitmsg_t;


//msg to camera
typedef struct __attribute__((packed)) {
    uint8_t default_;
    uint8_t compressionRatio;
    uint32_t pictureSize;
    float radians;
    float latitude;
    float longitude;
} nanosat_cammsg_t;

//reply to be filled by gps node
typedef struct __attribute__((packed)) {
    float latitude;
    float longitude;
    float altitude;
    unsigned int velocity;
} nanosat_gpsmsg_t;

/******************************************************************************
* union of all message -- for compaction
******************************************************************************/
typedef union __attribute__((packed)) {
    nanosat_datamsg_t datamsg;
    nanosat_exitmsg_t exitmsg;
    nanosat_cammsg_t cammsg;
} nanosat_data_t;

/******************************************************************************
* message put on wire
******************************************************************************/
typedef struct __attribute__((packed)) {
    uint8_t prefix;
    uint8_t type;
    uint8_t node_addr;
    uint8_t len;
    nanosat_data_t data;
    uint16_t checksum;
} nanosat_message_t;

//-----------------------------------------------------------
// Encode a Fletcher checksum in the last 2 bytes of buffer
// @todo document
//-----------------------------------------------------------
inline uint16_t fletcher_encode(uint8_t buffer[], long count )
{
    int i;
    unsigned char c0 = 0;
    unsigned char c1 = 0;
     *( buffer + count - 1 ) = 0;
     *( buffer + count - 2 ) = 0;
    for( i = 0; i < count; i++) {
        c0 = c0 + *( buffer + i );
        c1 = c1 + c0;
    }
    uint8_t hob = c0-c1;
    uint8_t lob = c1 - 2*c0;
    return (uint16_t)(hob<<8) | lob;
 }

//-------------------------------------------------------------
// Decode Fletcher Checksum. Returns zero if buffer error-free
// @todo document
//-------------------------------------------------------------
inline long fletcher_decode(uint8_t buffer[], long count )
{
    long result = 0;
    int i;
    unsigned char c0 = 0;
    unsigned char c1 = 0;
    for( i = 0; i < count; i++) {
        c0 = c0 + *( buffer + i );
        c1 = c1 + c0;
    }
    return((long)(c0 + c1)); // returns zero if buffer is error-free
}

#endif /* NANOSAT_MESSAGE_H */